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
#include <fstream>

using namespace boost::assign;

Simulation::Simulation() {
    /* Initialize random number generator */
    rng.seed(static_cast<boost::mt19937::result_type> (SEED));
}

Simulation::Simulation(const Simulation& orig) {
}

Simulation::~Simulation() {
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

VertSet Simulation::Metropolis(double lambda, double alpha) {
    MoveFactory m;

    for (int i = 0; i < 100; i++) // for testing
    {
        Move* move = m.createRandomMove(*this);

        // TODO: only generate valid moves
        if (!move->isMovePossible(vertices)) {
            continue;
        }

        /* acceptance = P(x') / P(x) * Q(x | x') / Q(x' | x) */
        double acceptance = move->getMoveProbability(lambda, alpha) *
                move->getInverseTransitionProbability(vertices) /
                move->getTransitionProbability(vertices);

        if (acceptance > 1 || getRandomNumber() < acceptance) {
            move->execute(vertices);
        }
    }

    // write a part of the grid to a file
    TriSet tri;
    collectTriangles(tri, *vertices.begin(), 2);
    drawPartialTriangulation("graph.dot", *vertices.begin(), tri);

    return vertices;
};

