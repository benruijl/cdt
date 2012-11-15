/* 
 * File:   Simulation.cpp
 * Author: ben
 * 
 * Created on September 21, 2012, 11:18 AM
 */

#include "Simulation.h"
#include "Utils.h"
#include "moves/MoveFactory.h"
#include "Boltzmann.h"
#include <boost/assign/std.hpp>
#include <boost/unordered_map.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/array.hpp>
#include <fstream>
#include <sstream>

using namespace boost::assign;

Simulation::Simulation() {
    /* Initialize random number generator */
    rng.seed(static_cast<boost::mt19937::result_type> (SEED));
}

Simulation::Simulation(const Simulation& orig) {
}

Simulation::~Simulation() {
    clearTriangulation();
}

void Simulation::clearTriangulation() {
    TriSet deleted;

    foreach(Vertex* v, vertices) {

        foreach(Triangle* t, v->getTriangles()) {
            if (deleted.find(t) == deleted.end()) {
                delete t;
                deleted.insert(t);
            }
        }
        delete v;
    }

    vertices.clear();
}

void Simulation::readFromFile(const char* filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Unable to open file '" << filename << "'" << std::endl;
        return;
    }

    char type;
    int vCount, vertA, vertB, vertC;

    file >> vCount;
    Vertex * vertex_array[vCount];

    clearTriangulation(); // clear old grid

    for (int i = 0; i < vCount; i++) {
        vertex_array[i] = new Vertex();
        vertices.push_back(vertex_array[i]);
    }

    // TODO: check if the triangulation is valid?
    while (file >> type >> vertA >> vertB >> vertC) {
        new Triangle(type == 'T' ? Triangle::TTS : Triangle::SST, vertex_array[vertA],
                vertex_array[vertB], vertex_array[vertC]);
    }

    file.close();
}

void Simulation::writeToFile(const char* filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Unable to open file '" << filename << "'" << std::endl;
        return;
    }

    int vCount = 0;
    typedef boost::unordered_map<Vertex*, int> vertex_map;
    vertex_map vertexMap;

    foreach(Vertex* v, vertices) {
        vertexMap.insert(vertex_map::value_type(v, vCount));
        vCount++;
    }

    file << vCount << std::endl;

    TriSet visitedTri;

    foreach(Vertex* v, vertices) {

        foreach(Triangle* t, v->getTriangles()) {
            if (visitedTri.find(t) != visitedTri.end()) {
                continue;
            }
            visitedTri.insert(t);

            if (t->getType() == Triangle::TTS) {
                file << "T";
            } else {
                file << "S";
            }

            for (int i = 0; i < 3; i++) {
                file << " " << vertexMap[t->getVertex(i)];
            }

            file << std::endl;
        }
    }

    file.close();
}

void Simulation::generateInitialTriangulation(int N, int T) {
    Vertex * vertices[N * T];
    Triangle * triangles[N * T * 2]; // TODO: remove, unnecessary

    /* Create vertices */
    for (int t = 0; t < T * N; t++) {
        vertices[t] = new Vertex();
        this->vertices.push_back(vertices[t]);
    }

    /* Create foliation */
    for (int t = 0; t < T; t++) {
        for (int s = 0; s < N; s++) {
            Triangle* u = new Triangle(Triangle::TTS, vertices[t * N + s], vertices[((t + 1) % T) * N + s],
                    vertices[t * N + (s + 1) % N]);
            Triangle* v = new Triangle(Triangle::TTS, vertices[((t + 1) % T) * N + (s + 1) % N], vertices[t * N + (s + 1) % N],
                    vertices[((t + 1) % T) * N + s]);
            triangles[t * 2 * N + 2 * s] = u;
            triangles[t * 2 * N + 2 * s + 1] = v;
        }
    }
}

Vertex* Simulation::getRandomVertex(const std::vector<Vertex*>& vertices) {
    BOOST_ASSERT(vertices.size() > 0);
    return vertices[unireal(rng) * vertices.size()];
}

template <typename T>
T Simulation::getRandomElementFromSet(const boost::unordered_set<T>& set) {
    typename boost::unordered_set<T>::iterator it = set.begin();

    if (set.size() == 0) {
        return NULL; // TODO: add assert?
    }

    boost::uniform_int<> uint(0, set.size() - 1); // TODO: check if slow?
    std::advance(it, uint(rng));
    return *it;
}

void Simulation::collectTriangles(TriSet& triSet, Vertex* v, int depth) {
    if (depth <= 0) {
        triSet += v->getTriangles();
        return;
    }

    triSet += v->getTriangles();

    foreach(Vertex* u, v->getNeighbouringVertices()) {
        collectTriangles(triSet, u, depth - 1);
    }
}

void Simulation::drawPartialTriangulation(const char* filename, Vertex* v, const TriSet& tri) {
    std::ofstream dotFile(filename);
    if (!dotFile.is_open()) {
        std::cout << "Unable to open file '" << filename << "'" << std::endl;
        return;
    }

    dotFile << "strict graph G {" << std::endl << "node[shape=point];" << std::endl;
    dotFile << "n" << v << "[color=green];" << std::endl;

    foreach(Triangle* t, tri) {
        for (int i = 0; i < 3; i++) {
            dotFile << "n" << t->getVertex(i) << "--" << "n" << t->getVertex((i + 1) % 3);
            if (t->isTimelike(i)) {
                dotFile << " [color=red];" << std::endl;
            } else {
                dotFile << " [color=blue];" << std::endl;
            }
        }
    }

    dotFile << "}";
    dotFile.close();
}

void Simulation::checkLinkOverlap() {
    for (int i = 0; i < vertices.size() - 1; i++) {
        for (int j = i + 1; j < vertices.size(); j++) {
            TriSet t = vertices[i]->getTriangles() & vertices[j]->getTriangles();

            if (t.size() != 2 && t.size() != 0) {
                std::cerr << "Link duplicates: " << i << " " << j << " " << t.size() << std::endl;
                BOOST_ASSERT(false);
            }
        }
    }
}

std::vector<int> Simulation::createID(Triangle* t) {
    typedef boost::tuple<Triangle*, Vertex*, Vertex*> curpos;
    std::vector<int> id;

    int newId = 8; // start at 8, 8 > character
    boost::unordered_map<Triangle*, int> tri;
    std::queue<curpos> neighbours;

    // add current triangle and first neighbour
    neighbours.push(
            boost::make_tuple(t, t->getVertex(0), t->getVertex(1)));
    neighbours.push(
            boost::make_tuple(t->getNeighbour(0), t->getVertex(0), t->getVertex(1)));

    while (!neighbours.empty()) {
        curpos cur = neighbours.front();
        neighbours.pop();

        Triangle* t = cur.get < 0 > ();

        boost::unordered_map<Triangle*, int>::iterator res = tri.find(t);
        if (res != tri.end()) {
            id.push_back(res->second);
            continue;
        }

        // create character of tri a - b - c
        Vertex* a = cur.get < 1 > ();
        Vertex* b = cur.get < 2 > ();
        Vertex* c = t->getThirdVertex(a, b);

        int character = t->isTimelike(a, b) * 4 + t->isTimelike(b, c) * 2 +
                t->isTimelike(c, a);

        id.push_back(character);
        tri[t] = newId;
        newId++;

        // add the two other neighbours to the stack
        neighbours.push(boost::make_tuple(t->getNeighbour(b, c),
                b, c));
        neighbours.push(boost::make_tuple(t->getNeighbour(c, a),
                c, a));
    }

    return id;
}

void Simulation::Metropolis(double lambda, double alpha, unsigned int numSweeps,
        unsigned int sweepLength) {
    unsigned long long moveRejectedBecauseImpossible = 0, moveRejectedBecauseDetBal = 0;
    MoveFactory m(*this);
    BoltzmannTester boltzmannTester;

    // choose a triangle that remains fixed
    // TODO: enforce this!
    Triangle* fixed = *vertices[0]->getTriangles().begin();

    for (unsigned long sweep = 0; sweep < numSweeps; sweep++) {

        /* Measure observables in the current state */
        foreach(Observable* o, observables) {
            o->measure(vertices);
        }

        for (int i = 0; i < sweepLength; i++) {
            Move* move = m.createRandomMove(*this);

            // some random moves can be impossible and to simplify the 
            // probability checks, we can do this explicit check
            if (!move->isMovePossible(vertices)) {
                //boltzmannTester.addStateId(createID(fixed));
                moveRejectedBecauseImpossible++;
                continue;
            }

            /* acceptance = P(x') / P(x) * Q(x | x') / Q(x' | x) */
            double acceptance = move->getMoveProbability(lambda, alpha) *
                    move->getInverseTransitionProbability(vertices) /
                    move->getTransitionProbability(vertices);

            /*std::cout << move->getMoveProbability(lambda, alpha) << " " <<
                    move->getInverseTransitionProbability(vertices) << " " <<
                    move->getTransitionProbability(vertices) << " " <<
                    acceptance;
             */
            if (acceptance > 1 || getRandomNumber() < acceptance) {
                move->execute(vertices);
            } else
                moveRejectedBecauseDetBal++;
            
            //boltzmannTester.addStateId(createID(fixed));
        }
    }

    /* Write some statistics*/
    std::cout << "Rejected impossible: " << moveRejectedBecauseImpossible
            << ", " << 100 * moveRejectedBecauseImpossible /
            ((float) sweepLength * (float) numSweeps) << "%" << std::endl;
    std::cout << "Rejected detailed balance: " << moveRejectedBecauseDetBal
            << ", " << 100 * moveRejectedBecauseDetBal /
            ((float) sweepLength * (float) numSweeps) << "%" << std::endl;
    
    //boltzmannTester.printFrequencies();

    // write a part of the grid to a file
    TriSet tri;
    collectTriangles(tri, *vertices.begin(), 1);
    drawPartialTriangulation("graph.dot", *vertices.begin(), tri);
};

