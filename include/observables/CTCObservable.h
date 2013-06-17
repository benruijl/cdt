/* 
 * File:   CTCObservable.h
 * Author: bruijl
 *
 * Created on June 13, 2013, 10:55 AM
 */

#ifndef CTCOBSERVABLE_H
#define	CTCOBSERVABLE_H

#include "Observable.h"
#include <vector>
#include "Vertex.h"

/**
 * Counts the number of non-contractible closed timelike curves
 * in a triangulation.
 * 
 * TODO: use Tarjan's strongly connected components algorithm?
 */
class CTCObservable : public Observable {
private:
    std::string filename;
    std::ofstream file;
    VertSet visited;
    
    VertSet loopsVisited; // used to count disjoint loops
    std::vector<Vertex*> loopCheck;
    unsigned int ctcCount;
    VertSet slice;

    std::vector<Vertex*> findShortestSlice(Vertex* start, Vertex* neighbour);

    void process(const std::vector<Vertex*>& state);

    void findLoop(Vertex* v, Vertex* prev);
public:

    CTCObservable(unsigned long writeFrequency);

    virtual ~CTCObservable() {

    }

    void printToScreen() {
        std::cout << "CTCs: " << ctcCount << std::endl;
    }

    /**
     * Prints the result of the computation to a file.
     */
    void printToFile() {
        file << ctcCount << std::endl;
    }
};

#endif	/* CTCOBSERVABLE_H */

