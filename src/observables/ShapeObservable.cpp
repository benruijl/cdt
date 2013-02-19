#include <boost/unordered/unordered_map.hpp>
#include <queue>
#include <deque>

#include "observables/ShapeObservable.h"
#include "Vertex.h"
#include "Triangle.h"
#include <utility>

ShapeObservable::ShapeObservable(unsigned long writeFrequency) :
Observable(writeFrequency, 0, true),
filename(createFilename("shape")),
file(filename.c_str()) {
}

ShapeObservable::~ShapeObservable() {
}

bool ShapeObservable::checkNonContractibility(
        const boost::unordered_set<std::pair<unsigned int, unsigned int> >& edge) {
    unsigned int t, s;
    t = (*edge.begin()).first;
    s = (*edge.begin()).second;

    std::queue<unsigned int> queueA, queueB;
    std::vector<char> visitedA(dualNeighbours.size()), 
            visitedB(dualNeighbours.size());
    unsigned int curA, curB;

    queueA.push(s);
    visitedA[s] = true;
    queueB.push(t);
    visitedB[t] = true;

    while (!queueA.empty() && !queueB.empty()) {
        curA = queueA.front();
        queueA.pop();

        curB = queueB.front();
        queueB.pop();

        for (int i = 0; i < 3; i++) {
            unsigned int nA = dualNeighbours[curA][i];
            unsigned int nB = dualNeighbours[curB][i];

            if (!visitedA[nA] &&
                    edge.find(std::make_pair(curA, nA)) == edge.end()) {
                queueA.push(nA);
                visitedA[nA] = true;

                if (visitedB[nA]) {
                    return true;
                }
            }

            if (!visitedB[nB] &&
                    edge.find(std::make_pair(curB, nB)) == edge.end()) {
                queueB.push(nB);
                 visitedB[nB] = true;

                if (visitedA[nB]) {
                    return true;
                }
            }
        }
    }

    return false;
}

void ShapeObservable::findNonContractibleLoop(const std::vector<Vertex*>& state,
        unsigned int start) {
    unsigned int cur;
    std::vector<unsigned int> prev(neighbours.size());
    std::queue<unsigned int> queue;
    queue.push(start);

    for (int i = 0; i < prev.size(); i++) {
        prev[i] = prev.size(); // prev.size() => no prev
    }

    while (!queue.empty()) {
        cur = queue.front();
        queue.pop();

        for (unsigned int k = 0; k < neighbours[cur].size(); k++) {
            unsigned int n = neighbours[cur][k];
            if (prev[n] == prev.size()) {
                queue.push(n);
                prev[n] = cur;
            } else { // loop found
                // loops in the same triangle are contractible
                if (prev[n] == prev[cur]) {
                    continue;
                }

                // TODO: eliminate obvious contractible loops
                unsigned int s = cur, t = n;
                boost::unordered_set<unsigned int> visited;
                std::deque<unsigned int> edge;

                while (true) {
                    if (s != prev.size()) {
                        visited.insert(s);
                        edge.insert(edge.begin(), s);
                        s = prev[s];
                    }

                    if (t != prev.size()) {
                        visited.insert(t);
                        edge.push_back(t);
                        t = prev[t];
                    }

                    if (s == t) {
                        visited.insert(s);
                        edge.insert(edge.begin(), s);
                        break;
                    }


                    if (visited.find(t) != visited.end()) {
                        // remove leading part from s branch
                        for (int i = 1; i < edge.size(); i++) {
                            if (edge[i] == t) {
                                edge.erase(edge.begin(), edge.begin() + i - 1);
                                break;
                            }
                        }
                        break;
                    }

                    if (visited.find(s) != visited.end()) {
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
                /*  for (int i = 0; i < edge.size(); i++) {
                      VertSet a = state[edge[i]]->getNeighbouringVertices();
                      BOOST_ASSERT(a.find(state[edge[(i + 1) % edge.size()]]) != a.end());
                  }*/

                boost::unordered_set<std::pair<unsigned int, unsigned int> >
                        edgeVertices;
                Triangle* q, *r;
                for (int i = 0; i < edge.size(); i++) {
                    Vertex::getAdjacentTriangles(state[edge[i]],
                            state[edge[(i + 1) % edge.size()]], &q, &r);

                    edgeVertices.insert(std::make_pair(triangleIds[q], triangleIds[r]));
                    edgeVertices.insert(std::make_pair(triangleIds[r], triangleIds[q]));
                }

                if (checkNonContractibility(edgeVertices)) {
                    size = edgeVertices.size() / 2;
                    //  std::cout << "Found non-contractible loop of length " <<
                    //          edgeVertices.size() / 2 << std::endl;

                    return;
                }
            }
        }
    }

    std::cout << "No non-contractible loops found..." << std::endl;
    BOOST_ASSERT(false);
}

void ShapeObservable::process(const std::vector<Vertex*>& state) {
    neighbours = buildLatticeConnectivity(state);
    triangleIds = createTriangleIds(state);
    dualNeighbours = buildDualLatticeConnectivity(triangleIds);

   // for (int i = 0; i < state.size(); i += state.size() / 10) {
        findNonContractibleLoop(state, 0);
   //     std::cout << "done " << i << std::endl;
        file << size << std::endl; // FIXME
   // }
}

void ShapeObservable::printToScreen() {
    std::cout << "Found non-contractible loop of length " <<
            size << std::endl;
}

void ShapeObservable::printToFile() {
    file << size << "\n";
}
