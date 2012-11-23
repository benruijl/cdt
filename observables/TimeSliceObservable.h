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

    typedef std::pair<unsigned int, unsigned int> averageLabel;
    typedef boost::unordered_map<Vertex*, averageLabel> vertexTimeLabel;
    vertexTimeLabel distance;
    VertSet timeslice; // time slice T = 0

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
            return std::pair<unsigned int, unsigned int>(0, 1);
        }

        vertexTimeLabel::iterator pos = distance.find(v);

        if (pos != distance.end()) {
            return pos->second;
        }

        /* Get the correct timelike sector */
        Triangle* t, *r;
        Vertex::getAdjacentTriangles(v, prev, &t, &r);
        VertSet tlSector = v->getSectorVertices(t, prev, true);
        unsigned int dist = 0, tot = 0;
        averageLabel p;

        foreach(Vertex* n, tlSector) {
            p = labelTime(n, v);
            dist += p.first + 1;
            tot += p.second;
        }

        averageLabel res(dist, tot);
        std::cout<< dist << " " << tot << std::endl;
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
        Vertex* curVertex = curTriangle->getVertex(1); // always has S link
        Vertex* edgeVertex = curTriangle->getType() == Triangle::TTS ?
                curTriangle->getVertex(2) : curTriangle->getVertex(0);

        while (timeslice.find(curVertex) == timeslice.end()) {
            order.push_back(curVertex);
            timeslice.insert(curVertex);


            // get spacelike vertices in other sector. Pick the first one.
            VertSet neighbours = curVertex->getSectorVertices(curTriangle, edgeVertex, false);

            edgeVertex = curVertex;
            curVertex = *neighbours.begin();
            Vertex::getAdjacentTriangles(curVertex, edgeVertex, &first, &second);
            curTriangle = first;
        }

        // TODO: if this is not the case, identify subloop and remove
        // vertices that are not in the subloop
        BOOST_ASSERT(curVertex == order[0]);
        
        std::cout << "ts " << timeslice.size() << std::endl;
        labelTime(curTriangle->getThirdVertex(curVertex, edgeVertex), curVertex);
        return;
        
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
        
        std::cout << nextslice.size() << std::endl;

        /* Generate labels for first slice, and as a result, this calculates
          labels for all slices */
        foreach(dirVertex p, nextslice) {
            labelTime(p.first, p.second);
        }

    }
public:

    TimeSliceObservable(unsigned long writeFrequency) :
    Observable(writeFrequency, 0, false),
    filename(createFilename("timelabel")) {
    }

    virtual ~TimeSliceObservable() {
    }

    void printToScreen() {
    }

    void printToFile() {
        std::ofstream file(filename.c_str());

        foreach(vertexTimeLabel::value_type v, distance) {
            file << v.first << " " << v.second.first / (double) v.second.second << "\n";
        }
        std::cout << "Done writing " << distance.size() << std::endl;
    }
};


#endif	/* TIMESLICEOBSERVABLE_H */

