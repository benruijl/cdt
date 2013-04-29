#include "observables/TimeSliceObservable.h"
#include "Vertex.h"
#include "Triangle.h"
#include <queue>

TimeSliceObservable::averageLabel TimeSliceObservable::labelTime(Vertex* v, Vertex* prev) {
    if (loopCheck.find(v) != loopCheck.end()) {
        std::cerr << "Timelike loop detected with same winding as spacelike loop"
                << ", skipping time labeling." << std::endl;
        throw (WINDING_ERROR);
    }

    if (timeslice.find(v) != timeslice.end()) {
        distance[v] = averageLabel(0.0, 1.0);
        return averageLabel(0.0, 1.0);
    }

    vertexTimeLabel::iterator pos = distance.find(v);

    if (pos != distance.end()) {
        return pos->second;
    }

    loopCheck.insert(v);

    /* Get the correct timelike sector */
    VertSet tlSector = v->getOtherSectorVertices(prev);
    double dist = 0, tot = 0;
    averageLabel p;

    foreach(Vertex* n, tlSector) {
        Triangle* first, *second;
        Vertex::getAdjacentTriangles(n, v, &first, &second);

        p = labelTime(n, v);
        dist += (p.first + 1) * p.second;
        tot += p.second;
    }

    averageLabel res(dist / tot, tot);
    distance[v] = res;
    loopCheck.erase(v);
    return res;
}

std::vector<Vertex*> TimeSliceObservable::findShortestSlice(Vertex* start, Vertex* neighbour) {
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

            do {
                path.push_back(p);
                p = prev[p];
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
    // have reached the starting node. However, if we go in the current direction,
    // we have the risk of entering another loop, so we turn the order around.
    // TODO: verify if this is always correct
    return findShortestSlice(cur.second, cur.first);
}

std::vector<Vertex*> TimeSliceObservable::createInitialSlice(Vertex* start) {
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

            if (bestTime < 0 || newSlice.size() < (unsigned int)bestTime) {
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

void TimeSliceObservable::process(const std::vector<Vertex*>& state) {
    maxTime = 0;
    distance.clear();
    loopCheck.clear();

    std::vector<Vertex*> order = createInitialSlice(state[0]);
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

    // vertex not in current slice
    sec = curTriangle->getThirdVertex(curVertex, sec);

    do {
        if (curTriangle->isTimelike(curVertex, sec)) {
            nextslice.insert(std::make_pair(sec, curVertex));
        }

        curTriangle = curTriangle->getNeighbour(curVertex, sec);

        // fix the propagation
        Vertex* third = curTriangle->getThirdVertex(curVertex, sec);

        // is the triangle trapped?
        if (timeslice.find(sec) != timeslice.end()) {
            std::cerr << "WARNING: trapped triangle, results may be inacurate" << std::endl;
            return;
        }

        if (timeslice.find(third) != timeslice.end()) {
            curVertex = third;
        } else {
            sec = third;
        }
    } while (curTriangle != start);

    /* Generate labels for first slice, and as a result, this calculates
      labels for all slices */
    foreach(dirVertex p, nextslice) {
        try {
            labelTime(p.first, p.second);
        } catch (int err) {
            return; // break up the calculations
        }

        double d = distance[p.first].first;

        if (d > maxTime) {
            maxTime = d;
        }
    }
}

TimeSliceObservable::TimeSliceObservable(unsigned long writeFrequency) :
Observable(writeFrequency, 0, true),
filename(createFilename("timelabel")) {
}

TimeSliceObservable::~TimeSliceObservable() {
}

void TimeSliceObservable::printToScreen() {
    std::cout << "Maximum time: " << maxTime << std::endl;
}

void TimeSliceObservable::printToFile() {
    // TODO: overwrite every time or have persistent storage?
    if (maxTime > 0) {
        std::ofstream file(filename.c_str());

        foreach(vertexTimeLabel::value_type v, distance) {
            file << v.first << " " << v.second.first << " " << v.second.second << "\n";
        }
    }
}
