/* 
 * File:   ConnectivityObservable.h
 * Author: bruijl
 *
 * Created on April 18, 2013, 4:30 PM
 */

#ifndef CONNECTIVITYOBSERVABLE_H
#define	CONNECTIVITYOBSERVABLE_H

#include "Observable.h"

class ConnectivityObservable : public Observable {
private:
    std::string filename;
    std::ofstream file;
    std::vector<unsigned int> connectivity;

    void process(const std::vector<Vertex*>& state) {
        connectivity.resize(state.size());
        for (unsigned int i = 0; i < state.size(); i++) {
            connectivity[i] = state[i]->getNeighbouringVertexCount();
        }
    }
public:

    ConnectivityObservable(unsigned long writeFrequency) :
    Observable(writeFrequency, 1, false),
    filename(createFilename("conn")),
    file(filename.c_str()) {
    }

    virtual ~ConnectivityObservable() {

    }

    void printToScreen() {
    }

    /**
     * Prints the result of the computation to a file.
     */
    void printToFile() {
        for (unsigned int i = 0; i < connectivity.size(); i++) {
            file << connectivity[i] << " ";
        }

        file << std::endl;
    }
};

#endif	/* CONNECTIVITYOBSERVABLE_H */

