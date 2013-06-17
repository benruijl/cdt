#include "observables/CTCObservable.h"
#include "Vertex.h"
#include "Triangle.h"
#include <queue>

CTCObservable::CTCObservable(unsigned long writeFrequency) :
Observable(writeFrequency, 1, true),
filename(createFilename("ctc")),
file(filename.c_str()) {
}

void CTCObservable::findLoop(Vertex* v, Vertex* prev) {
    if (slice.find(v) != slice.end()) {
        return;
    }

    // TODO: revert to set?
    // FIXME: does this find the same loop multiple times?
    for (unsigned int i = 0; i < loopCheck.size(); i++) {
        if (v == loopCheck[i]) {
            unsigned int overlap = 0;
            
            for (unsigned int j = i; j < loopCheck.size(); j++) {
                if (loopsVisited.find(loopCheck[j]) != loopsVisited.end()) {
                    overlap++;
                }
                
                loopsVisited.insert(loopCheck[j]);
            }
            
            if (overlap == 0) {
                ctcCount++;
            }
            
            //std::cout << "Found loop of length " << loopCheck.size() - i <<
             //       " with overlap " << overlap << std::endl;
        }
    }

    if (visited.find(v) != visited.end()) {
        return;
    }


    loopCheck.push_back(v);
    visited.insert(v); // TODO: before or after iterating neighbours?
    
    VertSet tlSector = v->getOtherSectorVertices(prev);

    foreach(Vertex* n, tlSector) {
        findLoop(n, v);
    }

    
    loopCheck.erase(loopCheck.end() - 1);

}

// FIXME: copied from TimeSliceObservable

std::vector<Vertex*> CTCObservable::findShortestSlice(Vertex* start, Vertex* neighbour) {
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

void CTCObservable::process(const std::vector<Vertex*>& state) {
    Triangle* t, *r;
    Vertex::getAdjacentTriangles(state[0], state[0]->getNeighbouringVertex(), &t, &r);
    VertSet sl = state[0]->getSectorVertices(t, true, false);
    std::vector<Vertex*> sliceO = findShortestSlice(state[0], *sl.begin());
    BOOST_ASSERT(sliceO.size() > 0);

    slice.clear();
    for (unsigned int i = 0; i < sliceO.size(); i++) {
        slice.insert(sliceO[i]);
    }

    loopsVisited.clear();
    visited.clear();
    ctcCount = 0;

    // find CTCs
    for (unsigned int i = 0; i < state.size(); i++) {
        Vertex::getAdjacentTriangles(state[i], state[i]->getNeighbouringVertex(), &t, &r);
        VertSet sl = state[i]->getSectorVertices(t, true, true);

        foreach(Vertex* n, sl) {
            findLoop(state[i], n);
        }
    }
}