/* 
 * File:   SpectralDimensionObservable.h
 * Author: bruijl
 *
 * Created on January 14, 2013, 3:33 PM
 */

#ifndef SPECTRALDIMENSIONOBSERVABLE_H
#define	SPECTRALDIMENSIONOBSERVABLE_H

#include <boost/unordered_map.hpp>
#include "Observable.h"

class SpectralDimensionObservable : public Observable {
private:
    std::string filename;
    std::ofstream file;
    double dimension;
    boost::unordered_map<unsigned int, double> specDim;

    void process(const std::vector<Vertex*>& state);
public:

    SpectralDimensionObservable(unsigned int writeFrequency);

    virtual ~SpectralDimensionObservable();

    void printToScreen();

    void printToFile();

};

#endif	/* SPECTRALDIMENSIONOBSERVABLE_H */

