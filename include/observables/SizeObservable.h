/* 
 * File:   SizeObservable.h
 * Author: bruijl
 *
 * Created on October 22, 2012, 11:03 AM
 */

#ifndef SIZEOBSERVABLE_H
#define	SIZEOBSERVABLE_H

#include "Observable.h"
#include <fstream>

/**
 *  Counts the number of vertices.
 */
class SizeObservable : public Observable {
private:
    unsigned int data;
    std::ofstream file;

    void process(const std::vector<Vertex*>& state) {
        data = state.size();
    }
public:

    SizeObservable(unsigned long writeFrequency, unsigned long registerFrequency) :
    Observable(writeFrequency, registerFrequency, true) { 
        std::string filename =  createFilename("size");
        file.open(filename.c_str());

        if (!file.is_open()) {
            std::cerr << "Could not open file '" << filename << "'" << std::endl;
        }
    }

    virtual ~SizeObservable() {
    }

    void printToScreen() {
        std::cout << "Sweep " << getMeasurementCount() << ", # Vertices: " << data << std::endl;
    }

    void printToFile() {
        file << data << std::endl; // fixme
    }
};


#endif	/* SIZEOBSERVABLE_H */

