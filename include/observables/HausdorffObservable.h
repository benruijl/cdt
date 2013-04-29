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
     * Calculates the distribution of number of new nodes in the annulus at 
     * distance r from a starting node.
     * @param neighbours Neighbour information
     * @param start Starting node
     * @return Distribution n(r)
     */
    std::vector<unsigned int> getDistribution(const NeighbourList& neighbours, unsigned int start);

    /**
     * Averages the distribution over `numSamples` randomly selected 
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

