/* 
 * File:   ShapeObservable.h
 * Author: bruijl
 *
 * Created on February 7, 2013, 11:48 AM
 */

#ifndef SHAPEOBSERVABLE_H
#define	SHAPEOBSERVABLE_H

#include "Observable.h"
#include "Vertex.h"

/**
 * Measures the extent of the small winding and big winding of the torus.
 */
class ShapeObservable : public Observable {
private:
    std::ofstream file;

    /**
     * Checks if a cycle is contractible by doing two simultaneous breadth-first
     * searches on either side of the edge and seeing if there is overlap.
     * @param edge Set of edge vertices
     * @param start A vertex in `edge`
     * @param sec Another vertex in `edge` and a neighbour of `start`
     */
    bool checkNonContractibility(const VertSet& edge, Vertex* start, Vertex* sec);
    
    void process(const std::vector<Vertex*>& state);
public:

    ShapeObservable(unsigned long writeFrequency);

    virtual ~ShapeObservable();

    void printToScreen();

    void printToFile();
};


#endif	/* SHAPEOBSERVABLE_H */

