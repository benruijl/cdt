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
    Observable("grid", writeFrequency, 1, false),
    simulation(simulation) {

    }

    virtual ~GridObservable() {

    }

    void measure(const std::vector<Vertex*>& state) {
        Observable::measure(state);
    }

    double getVariance(unsigned long n) {
        return 0.0;
    }

    void getLinearFit(unsigned long n, double& a, double& b) {

    }

    /**
     * Prints the result of the computation to the screen.
     */
    void printResult() {

    }

    /**
     * Prints the result of the computation to a file.
     */
    void printResult(const char* filename) {
        simulation->writeToFile(filename);
    }
};


#endif	/* GRIDOBSERVABLE_H */

