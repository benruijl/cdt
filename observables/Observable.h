/* 
 * File:   Observable.h
 * Author: bruijl
 *
 * Created on October 22, 2012, 10:59 AM
 */

#ifndef OBSERVABLE_H
#define	OBSERVABLE_H

#include <sstream>
#include <boost/date_time.hpp>

class Observable {
private:
    unsigned int registerFrequency, writeFrequency;
    unsigned int currentMeasurement;
    bool doPrintToScreen;
    const char* name;
    std::string timeStamp;

    /**
     * Process a given state.
     * @param state State
     */
    virtual void process(const std::vector<Vertex*>& state) = 0;
public:

    /**
     * Creates an observable. The timestamp appended to the output file is
     * the one at creation of this observable.
     * @param writeFrequency Sweep frequency to write the observed data to file.
     * @param registerFrequency Number of items to measure. Old items will be dropped.
     * 
     * TODO: currently registerFrequency is not used
     */
    Observable(unsigned int writeFrequency, unsigned int registerFrequency,
            bool printToScreen) :
    registerFrequency(registerFrequency),
    writeFrequency(writeFrequency),
    doPrintToScreen(printToScreen),
    currentMeasurement(0),
    name(name) {
        boost::posix_time::time_facet* facet = new boost::posix_time::time_facet("%d%m%Y_%H%M%S");
        boost::posix_time::ptime cur(boost::posix_time::second_clock::local_time());
        std::stringstream ss;
        ss.imbue(std::locale(ss.getloc(), facet));
        ss << cur;
        timeStamp = ss.str();
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
    unsigned int getMeasurementCount() {
        return currentMeasurement;
    }

    std::string getTimestamp() {
        return timeStamp;
    }

    /**
     * Creates a filename given a certain prefix. The filename includes
     * the timestamp.
     * @param prefix Prefix, for example 'size' or 'grid'
     * @return Filename, path included
     */
    std::string createFilename(const char* prefix) {
        std::stringstream ss;
        ss << "data/" << prefix << "_" << getTimestamp() << ".dat";
        return ss.str();
    }
};

#endif	/* OBSERVABLE_H */

