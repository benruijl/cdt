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

    void measure(const VertSet& state) {
        size.push_back(state.size());
    }

    double getVariance(int n) {
        if (n > size.size()) {
            n = size.size();
        }
        n = size.size() - n;

        double mean = 0;
        for (int i = n; i < size.size(); i++) {
            mean += size[i];
        }
        mean /= size.size();

        double var = 0;
        for (int i = n; i < size.size(); i++) {
            var += (size[i] - mean) * (size[i] - mean);
        }

        var /= (size.size() - n - 1);
    }

    void getLinearFit(int n, double& a, double& b) {
        double xy = 0, mx = 0, my = 0, sqmx = 0;

        n = size.size() - n;
        if (n < 0) {
            n = 0;
        }

        for (int i = n; i < size.size(); i++) {
            xy += i * size[i];
            mx += i;
            my += size[i];
            sqmx += i * i;
        }

        b = (xy - mx * my / (size.size() - n)) / 
                (sqmx - mx * mx / (size.size() - n));
        a = (my - b * mx) / (size.size() - n);
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

