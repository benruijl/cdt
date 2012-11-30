/* 
 * File:   TimeSliceObservable.h
 * Author: bruijl
 *
 * Created on November 21, 2012, 2:41 PM
 */

#ifndef TIMESLICEOBSERVABLE_H
#define	TIMESLICEOBSERVABLE_H

#include "Observable.h"
#include <fstream>
#include <boost/unordered_map.hpp>
#include <utility>

/**
 *  Tries to see if the topology of a 'time slice' that is 1 time unit removed
 * from a random initial time slice is that of a circle.
 */
class TimeSliceObservable : public Observable {
private:
    std::string filename;

    typedef std::pair<double, double> averageLabel;
    typedef boost::unordered_map<Vertex*, averageLabel> vertexTimeLabel;
    vertexTimeLabel distance;
    VertSet timeslice; // time slice T = 0
    double maxTime;

    VertSet fail;

    /**
     * Calculate time labels recursively.
     * @param v Vertex to find time of
     * @param prev Previous vertex, fixing time orientation. This can be seen
     * as the vertex from the future.
     * @return Pair of sum of timelike links required to get from start to this
     * vertex and the number of paths.
     *
     */
    averageLabel labelTime(Vertex* v, Vertex* prev) {
        if (fail.find(v) != fail.end()) {
            std::cout << fail.size() << std::endl;
            std::cout << v << " " << prev << std::endl;
            
            foreach(Vertex* l, timeslice) {
                std::cout << l << " ";
            }
            
            std::cout << std::endl;
            foreach(Vertex* l, fail) {
                std::cout << l << " ";
            }

            Simulation s;
            TriSet tri;
            s.collectTriangles(tri, v, 1);
            s.drawPartialTriangulation("graph.dot", v, tri);
            BOOST_ASSERT(false);
        }

        if (timeslice.find(v) != timeslice.end()) {
            return averageLabel(0.0, 1.0);
        }

        vertexTimeLabel::iterator pos = distance.find(v);

        if (pos != distance.end()) {
            return pos->second;
        }

        fail.insert(v);

        /* Get the correct timelike sector */
        VertSet tlSector = v->getOtherSectorVertices(prev);
        double dist = 0, tot = 0;
        averageLabel p;

        BOOST_ASSERT(tlSector.find(prev) == tlSector.end());

        foreach(Vertex* n, tlSector) {
            Triangle* first, *second;
            Vertex::getAdjacentTriangles(n, v, &first, &second);
            BOOST_ASSERT(first->isTimelike(v, n));

            p = labelTime(n, v);
            dist += (p.first + 1) * p.second;
            tot += p.second;
        }

        averageLabel res(dist / tot, tot);
        distance[v] = res;
        fail.erase(v);
        return res;
    }

    std::vector<Vertex*> createInitialSlice(Vertex* start) {
        timeslice.clear();

        /* Create an initial timeslice T = 0 */
        // TODO find shortest timeslice
        std::vector<Vertex*> order;

        Triangle* first, *second;
        Triangle* curTriangle = *start->getTriangles().begin();
        Vertex* curVertex = curTriangle->getVertex(0); // always has S link
        Vertex* edgeVertex = curTriangle->getType() == Triangle::TTS ?
                curTriangle->getVertex(2) : curTriangle->getVertex(1);

        while (timeslice.find(curVertex) == timeslice.end()) {
            order.push_back(curVertex);
            timeslice.insert(curVertex);

            // for testing
            BOOST_ASSERT(!curTriangle->isTimelike(curVertex, edgeVertex));


            // get spacelike vertices in other sector. Pick the first one.
            VertSet neighbours = curVertex->getOtherSectorVertices(edgeVertex);

            edgeVertex = curVertex;
            curVertex = *neighbours.begin();
            Vertex::getAdjacentTriangles(curVertex, edgeVertex, &first, &second);
            curTriangle = first;
            BOOST_ASSERT(!curTriangle->isTimelike(curVertex, edgeVertex));
        }

        // identify subloop and remove vertices that are not in the subloop
        if (order[0] != curVertex) {
            std::cout << "Adjusting spatial slice to be circular" << std::endl;
            std::cout << "tl_before " << order.size() << std::endl;

            timeslice.clear();
            for (int i = 0; i < order.size(); i++) {
                if (order[i] == curVertex) {
                    std::cout << "index: " << i << std::endl;

                    for (int j = i; j < order.size(); j++) {
                        timeslice.insert(order[j]);
                    }

                    // for testing
                    order.erase(order.begin(), order.begin() + i);
                    BOOST_ASSERT(timeslice.size() == order.size());
                    break;
                }

            }

            // test subloop
            for (int i = 1; i < order.size(); i++) {
                VertSet vs = order[i % order.size()]->getOtherSectorVertices(order[(i - 1) % order.size()]);
                BOOST_ASSERT(vs.find(order[(i + 1) % order.size()]) != vs.end());
            }

            std::cout << "check done" << std::endl;
        }
        
        return order;
    }

    void process(const std::vector<Vertex*>& state) {
        distance.clear();

        std::vector<Vertex*> order = createInitialSlice(state[0]);

        std::cout << "tl " << timeslice.size() << std::endl;

        typedef std::pair<Vertex*, Vertex*> dirVertex;

        /* Find all vertices in T = -1 */
        boost::unordered_set<dirVertex> nextslice;
        
        
        Vertex* curVertex = order[0];
        Vertex* sec = order[1];
        Triangle* t, *r;
        Vertex::getAdjacentTriangles(curVertex, sec, &t, &r);
        
        Triangle* curTriangle = t;
        Triangle* start = curTriangle;
        // vertex not in current slice
        sec = curTriangle->getThirdVertex(curVertex, sec);
        do {
            if (curTriangle->isTimelike(curVertex, sec)) {
                nextslice.insert(std::make_pair(sec, curVertex));
            }

            curTriangle = curTriangle->getNeighbour(curVertex, sec);

            // fix the propagation
            Vertex* third = curTriangle->getThirdVertex(curVertex, sec);
            if (timeslice.find(third) != timeslice.end()) {
                curVertex = third;
            } else {
                sec = third;
            }


        } while (curTriangle != start);


        maxTime = 0;

        /* Generate labels for first slice, and as a result, this calculates
          labels for all slices */
        foreach(dirVertex p, nextslice) {
            labelTime(p.first, p.second);

            double d = distance[p.first].first;

            if (d > maxTime) {
                maxTime = d;
            }
        }

    }
public:

    TimeSliceObservable(unsigned long writeFrequency) :
    Observable(writeFrequency, 0, true),
    filename(createFilename("timelabel")) {
    }

    virtual ~TimeSliceObservable() {
    }

    void printToScreen() {
        std::cout << "Maximum time: " << maxTime << std::endl;
    }

    void printToFile() {
        std::ofstream file(filename.c_str());

        foreach(vertexTimeLabel::value_type v, distance) {
            file << v.first << " " << v.second.first << " " << v.second.second << "\n";
        }
    }
};


#endif	/* TIMESLICEOBSERVABLE_H */

