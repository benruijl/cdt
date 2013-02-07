#include <boost/unordered/unordered_map.hpp>
#include <queue>

#include "observables/ShapeObservable.h"
#include "Vertex.h"
#include "Triangle.h"

ShapeObservable::ShapeObservable(unsigned long writeFrequency) :
Observable(writeFrequency, 0, true) {
}

ShapeObservable::~ShapeObservable() {
}

bool ShapeObservable::checkNonContractibility(const VertSet& edge, Vertex* start, Vertex* sec) {
    Triangle* s, *t;
    Vertex::getAdjacentTriangles(start, sec, &s, &t);

    std::queue<Triangle*> queueA, queueB;
    TriSet visitedA, visitedB;
    Triangle* curA, *curB;

    queueA.push(s);
    queueB.push(t);

    while (!queueA.empty() && !queueB.empty()) {
        curA = queueA.front();
        queueA.pop();

        curB = queueB.front();
        queueB.pop();

        for (int i = 0; i < 3; i++) {
            Triangle* nA = curA->getNeighbour(i);
            Triangle* nB = curB->getNeighbour(i);

            if (visitedA.find(nA) == visitedA.end() &&
                    edge.find(curA->getVertex(i)) == edge.end() &&
                    edge.find(curA->getVertex((i + 1) % 3)) == edge.end()) {
                queueA.push(nA);
                visitedA.insert(nA);

                if (visitedB.find(nA) != visitedB.end()) {
                    return false;
                }
            }

            if (visitedB.find(nB) == visitedB.end() &&
                    edge.find(curB->getVertex(i)) == edge.end() &&
                    edge.find(curB->getVertex((i + 1) % 3)) == edge.end()) {
                queueB.push(nB);
                visitedB.insert(nB);

                if (visitedA.find(nB) != visitedA.end()) {
                    return false;
                }
            }
        }
    }

    return true;
}

void ShapeObservable::process(const std::vector<Vertex*>& state) {
    /* Find a minimal non-contractible loop */
    Vertex* cur = state[0]; // TODO: start at multiple points
    boost::unordered_map<Vertex*, Vertex*> prev;
    std::queue<Vertex*> queue;
    
    while (!queue.empty()) {
        cur = queue.front();
        queue.pop();

        foreach(Vertex* n, cur->getNeighbouringVertices()) {
            if (prev.find(n) == prev.end()) {
                queue.push(n);
                prev[n] = cur;
            } else {
                // TODO: eliminate obvious contractible loops
                VertSet edge;
                edge.insert(n);
                while (cur != n) {
                    edge.insert(cur);
                    cur = prev[cur];
                }

                if (checkNonContractibility(edge, cur, n)) {
                    std::cout << "Found non-contractible loop of length " <<
                            edge.size() << std::endl;
                    return;
                }
            }
        }
    }

    std::cout << "No non-contractible loops found..." << std::endl;
}

void ShapeObservable::printToScreen() {
}

void ShapeObservable::printToFile() {
}
