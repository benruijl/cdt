/* 
 * File:   VolumeProfileObservable.h
 * Author: bruijl
 *
 * Created on January 9, 2013, 11:54 AM
 */

#ifndef VOLUMEPROFILEOBSERVABLE_H
#define	VOLUMEPROFILEOBSERVABLE_H

#include "Observable.h"

class TimeSliceObservable;
class Simulation;

/**
 *  This class counts the number of spacelike links in a cycle associated with
 * a certain time. The resulting graph is a volume profile. The time is the average
 * of the time of the vertices spanning this link. The spacelike cycles are sampled
 * from the triangulation and an attempt is made to make this uniform across the
 * time scale.
 * 
 * Note: data from `TimeSliceObservable` is used, so make sure to add the
 * `TimeSliceObservable` first to the simulation.
 */
class VolumeProfileObservable : public Observable {
private:
    std::string filename;
    Simulation* simulation;
    TimeSliceObservable* tso;
    std::vector<std::pair<double, unsigned int> > volumeProfile;
    
    void process(const std::vector<Vertex*>& state);
public:

    VolumeProfileObservable(unsigned int writeFrequency, TimeSliceObservable* tso,
            Simulation* simulation);

    virtual ~VolumeProfileObservable();

    void printToScreen();

    void printToFile();

};

#endif	/* VOLUMEPROFILEOBSERVABLE_H */

