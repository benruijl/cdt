#include <boost/unordered/unordered_map.hpp>
#include <queue>
#include <deque>

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
    visitedA.insert(s);
    queueB.push(t);
    visitedB.insert(t);

    while (!queueA.empty() && !queueB.empty()) {
        curA = queueA.front();
        queueA.pop();

        curB = queueB.front();
        queueB.pop();

        for (int i = 0; i < 3; i++) {
            Triangle* nA = curA->getNeighbour(i);
            Triangle* nB = curB->getNeighbour(i);

            if (visitedA.find(nA) == visitedA.end() &&
                    (edge.find(curA->getVertex(i)) == edge.end() ||
                    edge.find(curA->getVertex((i + 1) % 3)) == edge.end())) {
                queueA.push(nA);
                visitedA.insert(nA);

                if (visitedB.find(nA) != visitedB.end()) {
                    return true;
                }
            }

            if (visitedB.find(nB) == visitedB.end() &&
                    (edge.find(curB->getVertex(i)) == edge.end() ||
                    edge.find(curB->getVertex((i + 1) % 3)) == edge.end())) {
                queueB.push(nB);
                visitedB.insert(nB);

                if (visitedA.find(nB) != visitedA.end()) {
                    return true;
                }
            }
        }
    }

    return false;
}

void ShapeObservable::findNonContractibleLoop(const std::vector<Vertex*>& state) {
    /* Find a minimal non-contractible loop */
    Vertex* start = state[0], *cur; // TODO: start at multiple points
    boost::unordered_map<Vertex*, Vertex*> prev;
    std::queue<Vertex*> queue;
    queue.push(start);
    prev[start] = NULL;

    while (!queue.empty()) {
        cur = queue.front();
        queue.pop();

        foreach(Vertex* n, cur->getNeighbouringVertices()) {
            if (prev.find(n) == prev.end()) {
                queue.push(n);
                prev[n] = cur;
            } else {
                // TODO: eliminate obvious contractible loops
                Vertex* s = cur, *t = n;
                VertSet sEdge, tEdge;
                std::deque<Vertex*> edge;

                while (true) {
                    if (s != NULL) {
                        sEdge.insert(s);
                        edge.insert(edge.begin(), s);
                        s = prev[s];
                    }

                    if (t != NULL) {
                        tEdge.insert(t);
                        edge.push_back(t);
                        t = prev[t];
                    }

                    if (s == t) {
                        sEdge.insert(s);
                        edge.insert(edge.begin(), s);
                        break;
                    }


                    if (sEdge.find(t) != sEdge.end()) {
                        // remove leading part from s branch
                        for (int i = 1; i < edge.size(); i++) {
                            if (edge[i] == t) {
                                edge.erase(edge.begin(), edge.begin() + i - 1);
                                break;
                            }
                        }
                        break;
                    }

                    if (tEdge.find(s) != tEdge.end()) {
                        // remove trailing part from t branch
                        for (int i = edge.size() - 2; i >= 0; i--) {
                            if (edge[i] == s) {
                                edge.erase(edge.begin() + i + 1, edge.end());
                                break;
                            }
                        }
                        break;
                    }
                }

                if (edge.size() < 3) { // disallow pathological case
                    continue;
                }


                // std::cout << "edge size: " << edge.size() << std::endl;
                for (int i = 0; i < edge.size(); i++) {
                    VertSet a = edge[i]->getNeighbouringVertices();
                    BOOST_ASSERT(a.find(edge[(i + 1) % edge.size()]) != a.end());
                }

                sEdge.clear();
                for (int i = 0; i < edge.size(); i++) {
                    sEdge.insert(edge[i]);
                }

                if (checkNonContractibility(sEdge, cur, n)) {
                    std::cout << "Found non-contractible loop of length " <<
                            sEdge.size() << std::endl;
                    return;
                }
            }
        }
    }

    std::cout << "No non-contractible loops found..." << std::endl;
}

void ShapeObservable::process(const std::vector<Vertex*>& state) {
    findNonContractibleLoop(state);
}

void ShapeObservable::printToScreen() {
}

void ShapeObservable::printToFile() {
}
