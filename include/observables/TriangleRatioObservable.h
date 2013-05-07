/* 
 * File:   TriangleRatioObservable.h
 * Author: bruijl
 *
 * Created on May 7, 2013, 2:19 PM
 */

#ifndef TRIANGLERATIOOBSERVABLE_H
#define	TRIANGLERATIOOBSERVABLE_H

#include "Observable.h"

class TriangleRatioObservable : public Observable {
private:
    Simulation* simulation;
    std::string filename;
    std::ofstream file;

    void process(const std::vector<Vertex*>& state) {
        file << simulation->getTriangleRatio() << std::endl;
    }
public:

    TriangleRatioObservable(unsigned long writeFrequency, Simulation* simulation) :
    Observable(writeFrequency, 1, false),
    simulation(simulation),
    filename(createFilename("ratio")),
    file(filename.c_str()) {
    }

    virtual ~TriangleRatioObservable() {

    }

    void printToScreen() {
    }

    /**
     * Prints the result of the computation to a file.
     */
    void printToFile() {
    }
};

#endif	/* TRIANGLERATIOOBSERVABLE_H */

