/* 
 * File:   GridObservable.h
 * Author: bruijl
 *
 * Created on November 1, 2012, 11:09 AM
 */

#ifndef GRIDOBSERVABLE_H
#define	GRIDOBSERVABLE_H

#include "Simulation.h"

/**
 * Stores the grid.
 */
class GridObservable : public Observable {
private:
    Simulation& simulation;
public:

    GridObservable(Simulation& simulation, unsigned long writeFrequency) :
    Observable(writeFrequency, 1, false),
    simulation(simulation) {

    }

    virtual ~GridObservable() {

    }

    void printToScreen() {
    }

    /**
     * Prints the result of the computation to a file.
     */
    void printToFile() {
        simulation->writeToFile("grid.dat");
    }
};


#endif	/* GRIDOBSERVABLE_H */

