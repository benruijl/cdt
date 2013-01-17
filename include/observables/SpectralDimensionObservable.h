/* 
 * File:   SpectralDimensionObservable.h
 * Author: bruijl
 *
 * Created on January 14, 2013, 3:33 PM
 */

#ifndef SPECTRALDIMENSIONOBSERVABLE_H
#define	SPECTRALDIMENSIONOBSERVABLE_H

#include <boost/unordered_map.hpp>
#include <boost/array.hpp>
#include "Observable.h"

class SpectralDimensionObservable : public Observable {
private:
    static const unsigned int sigmaMax = 500; // TODO: make parameter
    static const unsigned int resetCount = 10;
    
    std::string filename;
    double dimension;
    boost::array<double, sigmaMax> prob;
    boost::array<double, sigmaMax> specDim;

    void process(const std::vector<Vertex*>& state);
public:

    SpectralDimensionObservable(unsigned int writeFrequency);

    virtual ~SpectralDimensionObservable();

    void printToScreen();

    void printToFile();

};

#endif	/* SPECTRALDIMENSIONOBSERVABLE_H */

