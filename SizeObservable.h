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
    std::vector<int> size;
public:

    SizeObservable() {

    }

    virtual ~SizeObservable() {

    }

    void measure(VertSet state) {
        size.push_back(state.size());
    }

    /**
     * Prints the result of the computation to the screen.
     */
    void printResult() {
        for (int i = 0; i < size.size(); i++)
            std::cout << size[i] << ",";
    }

    /**
     * Prints the result of the computation to a file.
     */
    void printResult(const char* filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cout << "Unable to open file '" << filename << "'" << std::endl;
        }

        for (int i = 0; i < size.size(); i++)
            file << size[i] << "\n";

        file.close();
    };
};


#endif	/* SIZEOBSERVABLE_H */

