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
#include "observables/Observable.h"

#include <boost/assign/std.hpp>
#include <boost/unordered_map.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/array.hpp>
#include <fstream>
#include <sstream>
#include <ctime>


using namespace boost::assign;

Simulation::Simulation() {
    /* Initialize random number generator */
    setSeed(std::time(0));
    moveFactory = new MoveFactory(*this);
}

Simulation::Simulation(const Simulation& orig) {
}

Simulation::~Simulation() {
    clearTriangulation();
    delete moveFactory;
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

    TTSCount = 0;
    SSTCount = 0;
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

        if (type == 'T') {
            TTSCount++;
        } else {
            SSTCount++;
        }
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

    TTSCount = N * T * 2;
    SSTCount = 0;
}

Vertex* Simulation::getRandomVertex(const std::vector<Vertex*>& vertices) {
    BOOST_ASSERT(vertices.size() > 0);
    boost::uniform_int<> uint(0, vertices.size() - 1);
    return vertices[uint(rng)];
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
    for (unsigned int i = 0; i < vertices.size() - 1; i++) {
        for (unsigned int j = i + 1; j < vertices.size(); j++) {
            TriSet t = vertices[i]->getTriangles() & vertices[j]->getTriangles();

            if (t.size() != 2 && t.size() != 0) {
                std::cerr << "Link duplicates: " << i << " " << j << " " << t.size() << std::endl;
                BOOST_ASSERT(false);
            }
        }
    }
}

void Simulation::printTriangleConnectivity(Triangle* t) {
    typedef boost::tuple<Triangle*, Vertex*, Vertex*> curpos;

    int newId = 0;
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
            std::cout << res->second << std::endl;
            continue;
        }

        // create character of tri a - b - c
        Vertex* a = cur.get < 1 > ();
        Vertex* b = cur.get < 2 > ();
        Vertex* c = t->getThirdVertex(a, b);

        std::cout << (t->isTimelike(a, b) ? "T" : "S") <<
                (t->isTimelike(b, c) ? "T" : "S") <<
                (t->isTimelike(c, a) ? "T" : "S") << ": " << newId << std::endl;

        tri[t] = newId;
        newId++;

        // add the two other neighbours to the stack
        neighbours.push(boost::make_tuple(t->getNeighbour(b, c),
                b, c));
        neighbours.push(boost::make_tuple(t->getNeighbour(c, a),
                c, a));
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

void Simulation::Metropolis(double alpha, unsigned int volume, double
        deltaVolume, unsigned int numSweeps, unsigned int sweepLength) {
    unsigned long long moveRejectedBecauseImpossible = 0, moveRejectedBecauseDetBal = 0;
    int bias = 0; // count the bias of the system size, left = -1, right = + 1
    double lambda = 5, lambdaLowerBound = 0, lambdaUpperBound = 10;

    /*BoltzmannTester boltzmannTester;
    Triangle* fixed = *vertices[0]->getTriangles().begin();
    moveFactory->setFixedTriangle(fixed);
    std::vector<int> id = createID(fixed);*/
    
    std::ofstream ratio("tri_ratio.dat"); // TODO: make observable

    for (unsigned long sweep = 0; sweep < numSweeps; sweep++) {
        if (sweep % 10 == 0) { // for testing
            //boltzmannTester.printFrequencies(lambda, alpha);
            
        }
        
        // for testing
        ratio << TTSCount / (double) SSTCount << " " << TTSCount << " " << SSTCount << " "
                << 2 * vertices.size() << std::endl;

        /* Measure observables in the current state */
        foreach(Observable* o, observables) {
            o->measure(vertices);
        }

        for (unsigned int i = 0; i < sweepLength; i++) {
            Move* move = moveFactory->createRandomMove(*this);

            // some random moves can be impossible and to simplify the 
            // probability checks, we can do this explicit check
            if (!move->isMovePossible(vertices)) {
                //boltzmannTester.addStateId(id);
                moveRejectedBecauseImpossible++;
                continue;
            }

            /* acceptance = P(x') / P(x) * Q(x | x') / Q(x' | x) */
            double acceptance = move->getMoveProbability(lambda, alpha) *
                    move->getInverseTransitionProbability(vertices) /
                    move->getTransitionProbability(vertices);

            /* add quadratic volume fixing term */
            double delta = move->getDeltaSST() + move->getDeltaTTS();
            acceptance *= exp(-deltaVolume * delta * (4.0 *
                    (double) vertices.size() + delta - 2.0 * (double) volume));

            if (acceptance > 1 || getRandomNumber() < acceptance) {
                move->execute(vertices);

                TTSCount += move->getDeltaTTS();
                SSTCount += move->getDeltaSST();
                //id = createID(fixed);
            } else
                moveRejectedBecauseDetBal++;

            if (2 * vertices.size() < volume)
                bias -= 1;
            if (2 * vertices.size() > volume)
                bias += 1;

            //boltzmannTester.addStateId(id);
        }

        /* Update lambda. Lower lambda means more growth, so the signs are
         * reversed. */
        if (bias > 0) {
            lambdaLowerBound = lambda;
        }
        if (bias < 0) {
            lambdaUpperBound = lambda;
        }
        
        lambda = 0.5 * (lambdaUpperBound + lambdaLowerBound);

        std::cout << "Lambda: " <<  lambda << ", bias: " << bias << std::endl;
        bias = 0;
    }

    /* Write some statistics*/
    std::cout << "Rejected impossible: " << moveRejectedBecauseImpossible
            << ", " << 100 * moveRejectedBecauseImpossible /
            ((float) sweepLength * (float) numSweeps) << "%" << std::endl;
    std::cout << "Rejected detailed balance: " << moveRejectedBecauseDetBal
            << ", " << 100 * moveRejectedBecauseDetBal /
            ((float) sweepLength * (float) numSweeps - (float) moveRejectedBecauseImpossible)
            << "%" << std::endl;
};

