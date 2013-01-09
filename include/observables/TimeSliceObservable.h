/* 
 * File:   TimeSliceObservable.h
 * Author: bruijl
 *
 * Created on November 21, 2012, 2:41 PM
 */

#ifndef TIMESLICEOBSERVABLE_H
#define	TIMESLICEOBSERVABLE_H

#include "Observable.h"
#include <fstream>
#include <boost/unordered_map.hpp>
#include "Vertex.h"

#define WINDING_ERROR 1

/**
 *  This class tries to identify a time for each vertex. It uses the average
 * distance to a time slice 
 */
class TimeSliceObservable : public Observable {
private:
    std::string filename;

    typedef std::pair<double, double> averageLabel;
    typedef boost::unordered_map<Vertex*, averageLabel> vertexTimeLabel;
    vertexTimeLabel distance;
    VertSet timeslice; // time slice T = 0
    double maxTime;

    VertSet loopCheck; // detect infinite loops

    /**
     * Calculate time labels recursively.
     * @param v Vertex to find time of
     * @param prev Previous vertex, fixing time orientation. This can be seen
     * as the vertex from the future.
     * @return Pair of sum of timelike links required to get from start to this
     * vertex and the number of paths.
     *
     */
    averageLabel labelTime(Vertex* v, Vertex* prev);

    /**
     * Creates an initial minimal spacelike slice, which attempts to include
     * vertex start. This may not be possible in all cases.
     * @param start Vertex to start search from
     * @return Spacelike slice
     */
    std::vector<Vertex*> createInitialSlice(Vertex* start);

    /**
     * Find shortest slice using breadth-first search. If the vertex start
     * is not a member of a slice, another slice in the search path is picked.
     * @param start Starting vertex
     * @param neighbour Neighbour of the vertex, specifying direction
     * @return Cycle
     */
    std::vector<Vertex*> findShortestSlice(Vertex* start, Vertex* neighbour);

    void process(const std::vector<Vertex*>& state);
public:

    TimeSliceObservable(unsigned long writeFrequency);

    virtual ~TimeSliceObservable();

    void printToScreen();

    void printToFile();

    /**
     * Gets the time label of a vertex.
     * @param v Vertex
     * @return Time label
     */
    double getTimeLabel(Vertex* v) {
        vertexTimeLabel::iterator it = distance.find(v);
        if (it == distance.end()) {
            return -1;
        }

        return it->second.first;
    }
};


#endif	/* TIMESLICEOBSERVABLE_H */

