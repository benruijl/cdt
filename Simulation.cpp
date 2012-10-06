/* 
 * File:   Simulation.cpp
 * Author: ben
 * 
 * Created on September 21, 2012, 11:18 AM
 */

#include "Simulation.h"
#include "Utils.h"
#include <boost/assign/std.hpp>

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

// FIXME: only moves that are not inverted

bool Simulation::isMovePossible(Moves::MOVES move, Vertex* u, Vertex* v) {
    Triangle* first, *second;
    Vertex::getAdjacentTriangles(u, v, &first, &second);

    switch (move) {
        case Moves::ALEXANDER_SPACELIKE:
            return !first->isTimelike(u, v);
        case Moves::ALEXANDER_TIMELIKE:
            return first->isTimelike(u, v);
        case Moves::COLLAPSE_SPACELIKE:
            return !first->isTimelike(u, v) && first->checkAdjacentSides(u, v)
                    && second->checkAdjacentSides(u, v);
        case Moves::COLLAPSE_TIMELIKE:
            return !first->isTimelike(u, v) && first->checkAdjacentSides(u, v)
                    && second->checkAdjacentSides(u, v);
        case Moves::FLIP:
        case Moves::FLIP_CHANGE:
            return !first->checkAdjacentSides(u, v) && !second->checkAdjacentSides(u, v);
    }
}

VertSet Simulation::getNeighbouringVertices(Vertex* v) {
    VertSet neighbours;

    foreach(Triangle* t, v->getTriangles()) {
        for (int i = 0; i < 3; i++) {
            neighbours.insert(t->getVertex(i));
        }
    }

    neighbours.erase(v);
    return neighbours;
}

Vertex* Simulation::getRandomVertex(VertSet& vertices) {
    VertSet::iterator it = vertices.begin();

    std::advance(it, unireal(rng) * vertices.size());
    return *it;
}

// TODO: support inverse moves

VertSet Simulation::Metropolis(double lambda, double alpha) {
    Moves m(vertices); // moves helper class, TODO: refactor

    // draw proposal
    Moves::MOVES move = m.generateRandomMove(unireal(rng));
    // is the move inverted? put to false for testing
    bool inv = false; // unireal(rng) < 0.5;
    double acceptance = m.getMoveProbability(move, inv, lambda, alpha);


    // select vertices
    Vertex* f = getRandomVertex(vertices);
    VertSet neighbours = getNeighbouringVertices(f);
    Vertex* g = getRandomVertex(neighbours);

    // acceptance *= Q(x | x') / Q(x' | x)
    // Q(x' | x) = 1 in our case
    acceptance *= m.getInverseMoveCount(move, f, g);


    if (acceptance > 1) {

    } else {
        if (unireal(rng) < acceptance) {
            // do move
        }
    }


    return vertices;
}

