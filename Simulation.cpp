/* 
 * File:   Simulation.cpp
 * Author: ben
 * 
 * Created on September 21, 2012, 11:18 AM
 */

#include "Simulation.h"
#include "Utils.h"
#include "MoveFactory.h"
#include <boost/assign/std.hpp>
#include <boost/unordered_map.hpp>
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
        vertices.insert(vertex_array[i]);
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
        this->vertices.insert(vertices[t]);
    }

    /* Create a foliation */
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

    /* Check causality to make sure the grid is correct */
    // TODO: remove from production release when satisfied with above algorithm
    for (int t = 0; t < T; t++) {
        for (int s = 0; s < N; s++) {
            if (!vertices[t * N + s]->checkCausality()) {
                std::cerr << "Causality failure at " << t << " " << s << std::endl;
            }
        }
    }

    /* Check if no links are double */
    for (int i = 0; i < N * T - 1; i++) {
        for (int j = i + 1; j < N * T; j++) {
            TriSet t = vertices[i]->getTriangles() & vertices[j]->getTriangles(); // intersection

            if (t.size() != 2 && t.size() != 0) {
                std::cerr << "Link duplicates: " << i << " " << j << " " << t.size() << std::endl;
            }
        }
    }

}

Vertex* Simulation::getRandomVertex(const VertSet& vertices) {
    BOOST_ASSERT(vertices.size() > 0);
    VertSet::iterator it = vertices.begin();

    std::advance(it, unireal(rng) * vertices.size());
    return *it;
}

template <typename T>
T Simulation::getRandomElementFromSet(const boost::unordered_set<T>& set) {
    typename boost::unordered_set<T>::iterator it = set.begin();

    if (set.size() == 0) {
        return NULL;
    }

    std::advance(it, unireal(rng) * set.size());
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

bool Simulation::checkLinkOverlap() {
    // TODO: improve, use that check if symmetric

    /* Check if no links are double */
    foreach(Vertex* a, vertices) {

        foreach(Vertex* b, vertices) {
            if (a == b) {
                continue;
            }
            TriSet t = a->getTriangles() & b->getTriangles(); // intersection

            if (t.size() != 2 && t.size() != 0) {
                std::cerr << "Link duplicates: " << a << " " << b << " " << t.size() << std::endl;
                BOOST_ASSERT(false);
            }
        }
    }
}

VertSet Simulation::Metropolis(double lambda, double alpha, int numIter) {
    MoveFactory m;

    // store which moves have been done
    std::ofstream moves("moves.dat");

    int moveRejectedBecauseImpossible = 0;
    // TODO: a convergence check should be added
    for (int i = 0; i < numIter; i++) {
        if (i % 100000 == 0) {
            std::cout << "Iteration " << i << std::endl;
            std::cout << "# Vertices " << vertices.size() << std::endl;
        }

        // output the grid after a certain amount of iterations
        // TODO: make observable
        if (i % 10000000 == 0) {
            std::ostringstream fn;
            fn << "grid_" << i / 10000000 << ".dat";
            const std::string filename = fn.str();
            writeToFile(filename.c_str());
        }

        Move* move = m.createRandomMove(*this);

        // some random moves can be impossible and to simplify the 
        // probability checks, we can 
        if (!move->isMovePossible(vertices)) {
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

            /* Measure observables from in the current state */
            foreach(Observable* o, observables) {
                o->measure(vertices);
            }

            moves << move->printID() << std::endl;
        }

        // Topological constraint
        // BOOST_ASSERT(vertices.size() >= 14 * 2);
    }

    moves.close();

    std::cout << "Rejected imp: " << moveRejectedBecauseImpossible << std::endl;

    // write a part of the grid to a file
    TriSet tri;
    collectTriangles(tri, *vertices.begin(), 1);
    drawPartialTriangulation("graph.dot", *vertices.begin(), tri);

    return vertices;
};

