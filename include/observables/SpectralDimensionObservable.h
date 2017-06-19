/* 
 * File:   SpectralDimensionObservable.h
 * Author: bruijl
 *
 * Created on January 14, 2013, 3:33 PM
 */

#ifndef SPECTRALDIMENSIONOBSERVABLE_H
#define	SPECTRALDIMENSIONOBSERVABLE_H

#include <boost/array.hpp>
#include "Observable.h"

class SpectralDimensionObservable : public Observable {
private:
    static constexpr double epsilon = 1e-9;
    unsigned int sigmaMax;
    unsigned int sampleSize;
    double diffusionConst; // diffusion constant
    bool dualLattice;
    
    typedef std::vector<double> Spec;
    typedef std::vector< std::vector<unsigned int> > NeighbourList;

    std::string filename;
    std::ofstream file;
    Spec specDim;
    Spec specDim1;

    void process(const std::vector<Vertex*>& state);
public:

    SpectralDimensionObservable(unsigned int writeFrequency);

    virtual ~SpectralDimensionObservable();

    void printToScreen();

    void printToFile();

};

#endif	/* SPECTRALDIMENSIONOBSERVABLE_H */

