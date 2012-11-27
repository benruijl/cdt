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

    /**
     * Calculate time labels recursively.
     * @param v Vertex to find time of
     * @param prev Previous vertex, fixing time orientation. This can be seen
     * as the vertex from the future.
     * @return Pair of sum of timelike links required to get from start to this
     * vertex and the number of paths.
     * 
     * TODO: check if second component can be left out.
     */
    averageLabel labelTime(Vertex* v, Vertex* prev) {
        if (timeslice.find(v) != timeslice.end()) {
            return averageLabel(0.0, 1.0);
        }

        vertexTimeLabel::iterator pos = distance.find(v);

        if (pos != distance.end()) {
            return pos->second;
        }

        /* Get the correct timelike sector */
        VertSet tlSector = v->getOtherSectorVertices(prev);
        double dist = 0, tot = 0;
        averageLabel p;

        foreach(Vertex* n, tlSector) {
            p = labelTime(n, v);
            dist += (p.first + 1) * p.second;
            tot += p.second;
        }

        averageLabel res(dist / tot, tot);
        distance[v] = res;
        return res;
    }

    void process(const std::vector<Vertex*>& state) {
        timeslice.clear();
        distance.clear();

        /* Create an initial timeslice T = 0 */
        // TODO make random
        std::vector<Vertex*> order;

        Triangle* first, *second;
        Triangle* curTriangle = *state[0]->getTriangles().begin();
        Vertex* curVertex = curTriangle->getVertex(0); // always has S link
        Vertex* edgeVertex = curTriangle->getType() == Triangle::TTS ?
                curTriangle->getVertex(2) : curTriangle->getVertex(1);

        while (timeslice.find(curVertex) == timeslice.end()) {
            order.push_back(curVertex);
            timeslice.insert(curVertex);

            // get spacelike vertices in other sector. Pick the first one.
            VertSet neighbours = curVertex->getOtherSectorVertices(edgeVertex);

            edgeVertex = curVertex;
            curVertex = *neighbours.begin();
            Vertex::getAdjacentTriangles(curVertex, edgeVertex, &first, &second);
            curTriangle = first;
        }

        // identify subloop and remove vertices that are not in the subloop
        if (order[0] != curVertex) {
            std::cout << "Adjusting spatial slice to be circular" << std::endl;

            std::vector<Vertex*>::iterator pos = std::find(order.begin(), order.end(), curVertex);
            order.erase(order.begin(), pos);
            timeslice.clear();

            foreach(Vertex* v, order) {
                timeslice.insert(v);
            }
        }

        typedef std::pair<Vertex*, Vertex*> dirVertex;

        /* Find all vertices in T = -1 */
        boost::unordered_set<dirVertex> nextslice;
        Triangle* start = curTriangle;
        // vertex not in current slice
        Vertex* sec = curTriangle->getThirdVertex(curVertex, edgeVertex);

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

