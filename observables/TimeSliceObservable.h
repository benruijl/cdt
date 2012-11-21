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

/**
 *  Tries to see if the topology of a 'time slice' that is 1 time unit removed
 * from a random initial time slice is that of a circle.
 */
class TimeSliceObservable : public Observable {
private:
    int sliceParts;

    void process(const std::vector<Vertex*>& state) {
        /* Create an initial timeslice T = 0 */
        std::vector<Vertex*> order;
        VertSet timeslice;

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
        BOOST_ASSERT((curVertex == order[0]));

        /* Find all vertices in T = 1 */
        VertSet nextslice;
        
        
    }
public:

    TimeSliceObservable(unsigned long writeFrequency) :
    Observable(writeFrequency, 0, true),
    sliceParts(0) {
    }

    virtual ~TimeSliceObservable() {
    }

    void printToScreen() {
        std::cout << "Distinct components: " << sliceParts << std::endl;
    }

    void printToFile() {
        // don't print to file
    }
};


#endif	/* TIMESLICEOBSERVABLE_H */

