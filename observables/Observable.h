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
    unsigned long registerFrequency, writeFrequency;
    unsigned long currentMeasurement;
    bool doPrintToScreen;
    const char* name;

    /**
     * Process a given state.
     * @param state State
     */
    virtual void process(const std::vector<Vertex*>& state) = 0;
public:

    /**
     * Creates an observable.
     * @param writeFrequency Frequency to write the observed data to file
     * @param registerFrequency Number of items to measure. Old items will be dropped.
     * 
     * TODO: currently registerFrequency is not used
     */
    Observable(unsigned long writeFrequency, unsigned long registerFrequency,
            bool printToScreen) :
    registerFrequency(registerFrequency),
    writeFrequency(writeFrequency),
    doPrintToScreen(printToScreen),
    currentMeasurement(0),
    name(name) {
    }

    virtual ~Observable() {

    }

    /**
     * Perform a measurement on a certain state. If the write frequency is met,
     * the data is written to a file, appended with the current observed number.
     * 
     * @param state System state
     */
    void measure(const std::vector<Vertex*>& state) {

        // should print to file?
        if (currentMeasurement > 0 && currentMeasurement % writeFrequency == 0) {
            process(state); // gather data
            
            if (doPrintToScreen) {
                printToScreen();
            }
            
            printToFile();
        }

        currentMeasurement++;
    }

    /**
     * Prints the last processed result to the screen.
     */
    virtual void printToScreen() = 0;

    /**
     *  Prints the last processed result to a file.
     */
    virtual void printToFile() = 0;

    /**
     * Get the number of the current measurement.
     */
    unsigned long getMeasurementCount() {
        return currentMeasurement;
    }
};

#endif	/* OBSERVABLE_H */

