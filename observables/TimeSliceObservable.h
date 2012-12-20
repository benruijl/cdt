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
#include <boost/heap/fibonacci_heap.hpp> // not available in scientific linux...
#include <utility>

/**
 *  Tries to see if the topology of a 'time slice' that is 1 time unit removed
 * from a random initial time slice is that of a circle.
 */
class TimeSliceObservable : public Observable {
private:
    std::string filename;

    typedef std::pair<double, double> averageLabel;
    typedef boost::unordered_map<Vertex*, averageLabel> vertexTimeLabel;
    vertexTimeLabel distance;
    VertSet timeslice; // time slice T = 0
    double maxTime;
    std::vector<Vertex*> vertices;

    VertSet fail;

    /**
     * Calculate time labels recursively.
     * @param v Vertex to find time of
     * @param prev Previous vertex, fixing time orientation. This can be seen
     * as the vertex from the future.
     * @return Pair of sum of timelike links required to get from start to this
     * vertex and the number of paths.
     *
     */
    averageLabel labelTime(Vertex* v, Vertex* prev) {
        if (fail.find(v) != fail.end()) {
            std::cout << fail.size() << std::endl;
            std::cout << v << " " << prev << std::endl;

            foreach(Vertex* l, timeslice) {
                std::cout << l << " ";
            }

            std::cout << std::endl;

            foreach(Vertex* l, fail) {
                std::cout << l << " ";
            }

            std::cout << std::endl;

            foreach(Vertex* a, vertices) {
                a->printConnectivity();
            }


            Simulation s;
            TriSet tri;
            s.collectTriangles(tri, v, 1);
            s.drawPartialTriangulation("graph.dot", v, tri);

            s.printTriangleConnectivity(*tri.begin());

            BOOST_ASSERT(false);
        }

        if (timeslice.find(v) != timeslice.end()) {
            return averageLabel(0.0, 1.0);
        }

        vertexTimeLabel::iterator pos = distance.find(v);

        if (pos != distance.end()) {
            return pos->second;
        }

        fail.insert(v);

        /* Get the correct timelike sector */
        VertSet tlSector = v->getOtherSectorVertices(prev);
        double dist = 0, tot = 0;
        averageLabel p;

        BOOST_ASSERT(tlSector.find(prev) == tlSector.end());

        foreach(Vertex* n, tlSector) {
            Triangle* first, *second;
            Vertex::getAdjacentTriangles(n, v, &first, &second);
            BOOST_ASSERT(first->isTimelike(v, n));

            p = labelTime(n, v);
            dist += (p.first + 1) * p.second;
            tot += p.second;
        }

        averageLabel res(dist / tot, tot);
        distance[v] = res;
        fail.erase(v);
        return res;
    }

    /**
     * Find shortest slice using breadth-first search. If the vertex start
     * is not a member of a slice, another slice in the search path is picked.
     * @param start Starting vertex
     * @param neighbour Neighbour of the vertex, specifying direction
     * @return Cycle
     */
    std::vector<Vertex*> findShortestSlice(Vertex* start, Vertex* neighbour) {
        typedef std::pair<Vertex*, Vertex*> linkDir;
        std::queue<linkDir> queue;
        VertSet visited;
        boost::unordered_map<Vertex*, Vertex*> prev;

        queue.push(linkDir(neighbour, start));
        prev[neighbour] = start;
        visited.insert(neighbour);
        
        linkDir cur;

        while (!queue.empty()) {
            cur = queue.front();
            queue.pop();

            if (cur.first == start) {
                // collect path
                Vertex* p = start;
                std::vector<Vertex*> path;

                int i = 0;
                do {
                    path.push_back(p);
                    p = prev[p];
                    i++;

                    BOOST_ASSERT(i < 30);
                } while (p != start);

                return path;
            }

            VertSet neighbours = cur.first->getOtherSectorVertices(cur.second);

            foreach(Vertex* n, neighbours) {
                if (visited.find(n) == visited.end()) {
                    visited.insert(n);
                    prev[n] = cur.first;
                    queue.push(linkDir(n, cur.first));
                }

            }
        }
        
        // path could not be found, this means that there is a subcycle.
        // the last searched node should be a part of this, or else it would
        // have reached the starting node. So run a new search from this node
        // TODO: verify if this is always correct
        return findShortestSlice(cur.first, cur.second);
    }

    std::vector<Vertex*> createInitialSlice(Vertex* start) {
        Triangle* t, *r;

        timeslice.clear();

        int bestTime = -1;
        std::vector<Vertex*> slice;

        /* For each outgoing vertex, find a shortest path */
        foreach(Vertex* n, start->getNeighbouringVertices()) {
            Vertex::getAdjacentTriangles(start, n, &t, &r);

            if (!t->isTimelike(n, start)) {
                std::vector<Vertex*> newSlice = findShortestSlice(n, start);

                if (newSlice.size() == 0) {
                    continue; // no path found
                }

                if (bestTime < 0 || newSlice.size() < bestTime) {
                    slice = newSlice;
                    bestTime = newSlice.size();
                }
            }
        }

        foreach(Vertex* v, slice) {
            timeslice.insert(v);
        }

        return slice;
    }

    void process(const std::vector<Vertex*>& state) {
        vertices = state;
        distance.clear();

        std::vector<Vertex*> order = createInitialSlice(state[0]);

        if (order.size() == 0) {
            foreach(Vertex* a, state) {
                a->printConnectivity();
            }
            BOOST_ASSERT(false);
        }

        std::cout << "T=0 size: " << timeslice.size() << std::endl;

        typedef std::pair<Vertex*, Vertex*> dirVertex;

        /* Find all vertices in T = -1 */
        boost::unordered_set<dirVertex> nextslice;


        Vertex* curVertex = order[0];
        Vertex* sec = order[1];
        Triangle* t, *r;
        Vertex::getAdjacentTriangles(curVertex, sec, &t, &r);

        Triangle* curTriangle = t;
        Triangle* start = curTriangle;

        BOOST_ASSERT(timeslice.find(curVertex) != timeslice.end() &&
                timeslice.find(sec) != timeslice.end());

        // vertex not in current slice
        sec = curTriangle->getThirdVertex(curVertex, sec);


        int c = 0;
        do {
            BOOST_ASSERT(timeslice.find(curVertex) != timeslice.end());

            if (curTriangle->isTimelike(curVertex, sec)) {
                nextslice.insert(std::make_pair(sec, curVertex));
            }

            curTriangle = curTriangle->getNeighbour(curVertex, sec);

            // fix the propagation
            Vertex* third = curTriangle->getThirdVertex(curVertex, sec);

            // is the triangle trapped?
            if (timeslice.find(sec) != timeslice.end()) {
                std::cerr << "WARNING: trapped triangle, results may be inacurate" << std::endl;
                break;
            }

            if (timeslice.find(third) != timeslice.end()) {
                curVertex = third;
            } else {
                sec = third;
            }

            c++;

            BOOST_ASSERT(c < 80);
        } while (curTriangle != start);


        maxTime = 0;

        /* Generate labels for first slice, and as a result, this calculates
          labels for all slices */
        foreach(dirVertex p, nextslice) {
            labelTime(p.first, p.second);

            double d = distance[p.first].first;

            if (d > maxTime) {
                maxTime = d;
            }
        }

    }
public:

    TimeSliceObservable(unsigned long writeFrequency) :
    Observable(writeFrequency, 0, true),
    filename(createFilename("timelabel")) {
    }

    virtual ~TimeSliceObservable() {
    }

    void printToScreen() {
        std::cout << "Maximum time: " << maxTime << std::endl;
    }

    void printToFile() {
        std::ofstream file(filename.c_str());

        foreach(vertexTimeLabel::value_type v, distance) {
            file << v.first << " " << v.second.first << " " << v.second.second << "\n";
        }
    }
};


#endif	/* TIMESLICEOBSERVABLE_H */

