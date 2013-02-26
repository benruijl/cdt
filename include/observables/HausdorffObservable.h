/* 
 * File:   HausdorffObservable.h
 * Author: bruijl
 *
 * Created on February 18, 2013, 10:25 AM
 */

#ifndef HAUSDORFFOBSERVABLE_H
#define	HAUSDORFFOBSERVABLE_H

#include "Observable.h"
#include "Utils.h"

class HausdorffObservable : public Observable {
private:
    std::string filename;
    std::ofstream file;
    std::pair<unsigned int, double> extent;

    /**
     * Calculates the average linear extent sum (r * area(r)) where area(r)
     * is the number of nodes inside a disc or radius r starting at `start`.
     * @param neighbours Neighbour information
     * @param start Index in `neighbours` that gives the starting position.
     * @return Average linear extent
     */
    double getExtent(NeighbourList& neighbours, unsigned int start);
    
    /**
     * Averages the linear extent over every starting vertex and stores it in
     * `extent`.
     * @param state
     */
    void process(const std::vector<Vertex*>& state);
public:

    HausdorffObservable(unsigned long writeFrequency);

    virtual ~HausdorffObservable();

    void printToScreen();

    void printToFile();
};


#endif	/* HAUSDORFFOBSERVABLE_H */

