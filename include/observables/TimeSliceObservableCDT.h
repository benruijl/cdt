/* 
 * File:   TimeSliceObservableCDT.h
 * Author: bruijl
 *
 * Created on May 14, 2013, 11:35 AM
 */

#ifndef TIMESLICEOBSERVABLECDT_H
#define	TIMESLICEOBSERVABLECDT_H

#include "Observable.h"

/**
 * A time slice observable specialized for CDT. It outputs the volume profile
 * for a CDT triangulation.
 * 
 * Note that the starting point T = 0 can drift, because it is chosen as the
 * first element in the vertex list.
 * 
 * FIXME: rename to VolumeProfileObservableCDT?
 */
class TimeSliceObservableCDT : public Observable {
private:
    std::string filename;
    std::ofstream file;
    
    unsigned int T; // number of time steps
    std::vector<unsigned int> volumeProfile;

    void process(const std::vector<Vertex*>& state);
public:

    /**
     * Creates a new time slice observable, for CDT.
     * @param writeFrequency Measure frequency
     * @param T Number of time steps. This is fixed in CDT
     */
    TimeSliceObservableCDT(unsigned int writeFrequency, unsigned int T);

    virtual ~TimeSliceObservableCDT();

    void printToScreen();

    void printToFile();

};

#endif	/* TIMESLICEOBSERVABLECDT_H */

