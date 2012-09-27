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

Vertex* Simulation::doCollapse(Vertex* a, Vertex* b) {
    // TODO: check if the side links are of the same type
    // if not, the replacement is invalid

    // find triangles that can be removed
    Triangle* first, *second;
    Vertex::getAdjacentTriangles(a, b, &first, &second);


    std::cout << "Old count: " << b->getTriangles().size() << std::endl; // old number

    first->removeVertices();
    second->removeVertices();

    b->getTriangles() += a->getTriangles();

    std::cout << "New count: " << b->getTriangles().size() << std::endl;

    foreach(Triangle* t, a->getTriangles()) {
        t->replaceVertex(a, b);
    }

    free(a);
    free(first);
    free(second);

    BOOST_ASSERT(b->checkCausality());

    return b;
}

Vertex* Simulation::doMove(Vertex* a, Vertex* b, MOVES move) {
    switch (move) {
        case COLLAPSE:
            return doCollapse(a, b);
    };

    return NULL;
}

Triangle* Simulation::generateInitialTriangulation(int N, int T) {
    Vertex * vertices[N * T];
    Triangle * triangles[N * T * 2];

    /* Create vertices */
    for (int t = 0; t < T * N; t++) {
        vertices[t] = new Vertex();
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

    return triangles[0];
}

Triangle * Simulation::Metropolis(Triangle * triangulation) {
    return triangulation;
}

