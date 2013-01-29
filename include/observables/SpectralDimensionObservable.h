/* 
 * File:   SpectralDimensionObservable.h
 * Author: bruijl
 *
 * Created on January 14, 2013, 3:33 PM
 */

#ifndef SPECTRALDIMENSIONOBSERVABLE_H
#define	SPECTRALDIMENSIONOBSERVABLE_H

#include <boost/array.hpp>
#include <boost/circular_buffer.hpp>
#include "Observable.h"

class SpectralDimensionObservable : public Observable {
private:
    static const double epsilon = 1e-9;
    static const unsigned int sigmaMax = 1000; // TODO: make parameter
    static const unsigned int sampleSize = 6000;
    typedef boost::array<double, sigmaMax> Spec;
    typedef std::vector< std::vector<unsigned int> > NeighbourList;

    std::string filename;
    double dimension;
    Spec prob;
    boost::circular_buffer<Spec> specDim;
    boost::circular_buffer<Spec> specDim1;

    void process(const std::vector<Vertex*>& state);

    /**
     * Because the spectral dimension measurement is slow, a list is built that maps
     * connectivity.
     * @param state
     */
    NeighbourList buildConnectivity(const std::vector<Vertex*>& state);
public:

    SpectralDimensionObservable(unsigned int writeFrequency);

    virtual ~SpectralDimensionObservable();

    void printToScreen();

    void printToFile();

};

#endif	/* SPECTRALDIMENSIONOBSERVABLE_H */

