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

class Simulation;

class HausdorffObservable : public Observable {
private:
    std::string filename;
    std::ofstream file;
    unsigned int numSamples;
    Simulation* simulation;
    std::vector<double> dist;
    std::pair<unsigned int, double> extent;

    /**
     * Calculates the average linear extent sum (r * area(r)) where area(r)
     * is the number of nodes inside a disc or radius r starting at `start`.
     * @param neighbours Neighbour information
     * @param start Index in `neighbours` that gives the starting position.
     * @return Average linear extent
     */
    double getExtent(std::vector<unsigned int>& area, unsigned int norm);
    
    std::vector<unsigned int> getDistribution(NeighbourList& neighbours, unsigned int start);

    /**
     * Averages the linear extent over `numSamples` randomly selected 
     * starting vertices and stores it in `extent`. 
     * @param state
     */
    void process(const std::vector<Vertex*>& state);
public:

    HausdorffObservable(Simulation* simulation, unsigned long writeFrequency);

    virtual ~HausdorffObservable();

    void printToScreen();

    void printToFile();
};


#endif	/* HAUSDORFFOBSERVABLE_H */

