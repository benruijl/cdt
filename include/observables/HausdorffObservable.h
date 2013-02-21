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
    std::vector<unsigned int> area;
    std::pair<unsigned int, double> extent;

    double getExtent(NeighbourList& neighbours, unsigned int start);
    void process(const std::vector<Vertex*>& state);
public:

    HausdorffObservable(unsigned long writeFrequency);

    virtual ~HausdorffObservable();

    void printToScreen();

    void printToFile();
};


#endif	/* HAUSDORFFOBSERVABLE_H */

