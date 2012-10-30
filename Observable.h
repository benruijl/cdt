/* 
 * File:   Observable.h
 * Author: bruijl
 *
 * Created on October 22, 2012, 10:59 AM
 */

#ifndef OBSERVABLE_H
#define	OBSERVABLE_H

#include <sstream>

class Observable {
private:
    int registerFrequency, writeFrequency;
    int currentMeasurement;
    const char* name;
public:

    /**
     * Creates an observable.
     * @param name Name of observable, will be prepended to filename
     * @param writeFrequency Frequency to write the observed data to file
     * @param registerFrequency Number of items to measure. Old items will be dropped.
     */
    Observable(const char* name, int writeFrequency, int registerFrequency) :
    registerFrequency(registerFrequency),
    writeFrequency(writeFrequency),
    currentMeasurement(0),
    name(name) {
    }

    virtual ~Observable() {

    }

    /**
     * Perform a measurement on a certain state. If the write frequency is met,
     * the data is written to a file, appended with the current observed number.
     * 
     * This function is to be called by derived classes at the end of a measurement.
     * 
     * @param state System state
     */
    virtual void measure(const VertSet& state) {

        // should print to file?
        if (currentMeasurement > 0 && currentMeasurement % writeFrequency == 0) {
            std::ostringstream fn;
            fn << name << "_" << currentMeasurement / writeFrequency << ".dat";
            const std::string filename = fn.str();
            printResult(filename.c_str());
        }

        currentMeasurement++;
    }

    /**
     * Prints the result of the computation to the screen.
     */
    virtual void printResult() = 0;

    /**
     * Prints the result of the computation to a file.
     */
    virtual void printResult(const char* filename) = 0;

    /**
     * Gets the variance of the <b>last</b> n measurements.
     * @param n Should be less than total observations
     */
    virtual double getVariance(int n) = 0;

    virtual void getLinearFit(int n, double& a, double& b) = 0;
};

#endif	/* OBSERVABLE_H */

