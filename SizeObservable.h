/* 
 * File:   SizeObservable.h
 * Author: bruijl
 *
 * Created on October 22, 2012, 11:03 AM
 */

#ifndef SIZEOBSERVABLE_H
#define	SIZEOBSERVABLE_H

#include "Observable.h"
#include <boost/circular_buffer.hpp>
#include <fstream>

/**
 *  Counts the number of vertices.
 */
class SizeObservable : public Observable {
private:
    boost::circular_buffer<int> data; // measured data
public:

    SizeObservable(int writeFrequency, int registerFrequency) :
    Observable("size", writeFrequency, registerFrequency),
    data(registerFrequency) {

    }

    virtual ~SizeObservable() {

    }

    void measure(const std::vector<Vertex*>& state) {
        data.push_back(state.size());
        Observable::measure(state);
    }

    double getVariance(int n) {
        if (n > data.size()) {
            n = data.size();
        }
        n = data.size() - n;

        double mean = 0;
        for (int i = n; i < data.size(); i++) {
            mean += data[i];
        }
        mean /= data.size();

        double var = 0;
        for (int i = n; i < data.size(); i++) {
            var += (data[i] - mean) * (data[i] - mean);
        }

        var /= (data.size() - n - 1);
    }

    void getLinearFit(int n, double& a, double& b) {
        double xy = 0, mx = 0, my = 0, sqmx = 0;

        n = data.size() - n;
        if (n < 0) {
            n = 0;
        }

        for (int i = n; i < data.size(); i++) {
            xy += i * data[i];
            mx += i;
            my += data[i];
            sqmx += i * i;
        }

        b = (xy - mx * my / (data.size() - n)) /
                (sqmx - mx * mx / (data.size() - n));
        a = (my - b * mx) / (data.size() - n);
    }

    /**
     * Prints the result of the computation to the screen.
     */
    void printResult() {
        for (int i = 0; i < data.size(); i++)
            std::cout << data[i] << ",";
    }

    /**
     * Prints the result of the computation to a file.
     */
    void printResult(const char* filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cout << "Unable to open file '" << filename << "'" << std::endl;
        }

        for (int i = 0; i < data.size(); i++)
            file << data[i] << "\n";

        file.close();
    };
};


#endif	/* SIZEOBSERVABLE_H */

