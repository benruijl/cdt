#include "Utils.h"
#include "Vertex.h"
#include "Triangle.h"

#include <boost/unordered_map.hpp>
#include <deque>

NeighbourList buildLatticeConnectivity(const std::vector<Vertex*>& state) {
    boost::unordered_map<Vertex*, unsigned int> index;
    index.rehash(state.size());
    NeighbourList neighbours(state.size());

    for (unsigned int i = 0; i < state.size(); i++) {
        index[state[i]] = i;
    }

    for (unsigned int i = 0; i < state.size(); i++) {

        foreach(Vertex* n, state[i]->getNeighbouringVertices()) {
            neighbours[i].push_back(index[n]);
        }
    }

    return neighbours;
}

NeighbourList buildDualLatticeConnectivity(boost::unordered_map<Triangle*, unsigned int>& index) {
    NeighbourList neighbours(index.size());
    typedef std::pair<Triangle*, unsigned int> triIdPair;

    foreach(const triIdPair& p, index) {
        for (int i = 0; i < 3; i++) {
            neighbours[p.second].push_back(index[p.first->getNeighbour(i)]);
        }
    }

    return neighbours;
}

NeighbourList buildDualLatticeConnectivity(const std::vector<Vertex*>& state) {
    boost::unordered_map<Triangle*, unsigned int> ids = createTriangleIds(state);
    return buildDualLatticeConnectivity(ids);
}

boost::unordered_map<Triangle*, unsigned int> createTriangleIds(const std::vector<Vertex*>& state) {
    boost::unordered_map<Triangle*, unsigned int> index;
    index.rehash(state.size() * 2);

    int id = 0;
    for (unsigned int i = 0; i < state.size(); i++) {

        foreach(Triangle* t, state[i]->getTriangles()) {
            if (index.find(t) == index.end()) {
                index[t] = id;
                id++;
            }
        }
    }

    return index;
}

bool hasSelfOverlappingBubbles(Vertex* a, Vertex* b) {
    typedef std::pair<Vertex*, Vertex*> curPrev;

    boost::unordered_map<Vertex*, Vertex*> visited;
    std::deque<curPrev> queue;

    queue.push_back(curPrev(a, b));
    queue.push_back(curPrev(b, a));

    while (!queue.empty()) {
        curPrev c = queue.back();
        queue.pop_back();

        VertSet neigh = c.first->getSameSectorVertices(c.second);
        boost::unordered_map<Vertex*, Vertex*>::iterator pos = visited.find(c.first);

        if (pos != visited.end()) {
            // check if the hit is due to compactification or overlap
            if (neigh.find(pos->second) == neigh.end()) {
                return true; // found overlap
            }

            // already visited, so continue to next queue element
            continue;
        }

        visited[c.first] = c.second;

        foreach(Vertex* n, neigh) {
            if (n != c.second) {
                queue.push_back(curPrev(n, c.first));
            }
        }
    }

    return false;
}