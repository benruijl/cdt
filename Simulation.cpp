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

// TODO: also create the inverse process

Vertex* Simulation::doCollapse(Vertex* a, Vertex* b) {
    Triangle* first, *second;
    Vertex::getAdjacentTriangles(a, b, &first, &second);

    /* A collapse is only valid if the to be merged links are of the same type */
    BOOST_ASSERT(first->checkAdjacentSides(a, b));
    BOOST_ASSERT(second->checkAdjacentSides(a, b));

    first->removeVertices();
    second->removeVertices();

    b->getTriangles() += a->getTriangles();

    foreach(Triangle* t, a->getTriangles()) {
        t->replaceVertex(a, b);
    }

    free(a);
    free(first);
    free(second);

    BOOST_ASSERT(b->checkCausality());

    return b;
}

// TODO: add second possibility

Vertex* Simulation::doFlip(Vertex* a, Vertex* b) {
    Triangle* first, *second;
    Vertex::getAdjacentTriangles(a, b, &first, &second);

    Vertex* c = first->getThirdVertex(a, b);
    Vertex* d = second->getThirdVertex(a, b);

    a->getTriangles().erase(second);
    c->getTriangles().insert(second);
    second->replaceVertex(a, c);

    b->getTriangles().erase(first);
    d->getTriangles().insert(first);
    first->replaceVertex(b, d);

    BOOST_ASSERT(a->checkCausality());
    BOOST_ASSERT(b->checkCausality());
    BOOST_ASSERT(c->checkCausality());
    BOOST_ASSERT(d->checkCausality());

    return c;
}

Vertex* Simulation::doAlexander(Vertex* a, Vertex* b) {
    Triangle* first, *second;
    Vertex::getAdjacentTriangles(a, b, &first, &second);

    Vertex* c = first->getThirdVertex(a, b);
    Vertex* d = second->getThirdVertex(a, b);
    
    /* Get link types */
    bool lAB = first->isTimelike(a, b);
    bool lAC = first->isTimelike(a, c);
    bool lCB = first->isTimelike(c, b);
    bool lAD = second->isTimelike(a, d);
    bool lBD = second->isTimelike(b, d);
    bool newLink = !lAB;

    Vertex* u = new Vertex(); // new vertex
    new Triangle(a, c, u, lAC, newLink, lAB);
    new Triangle(u, c, b, newLink, lCB, lAB);
    new Triangle(a, u, d, lAB, newLink, lAD);
    new Triangle(u, b, d, lAB, lBD, newLink);
    
    first->removeVertices();
    second->removeVertices();
    free(first);
    free(second);

    BOOST_ASSERT(a->checkCausality());
    BOOST_ASSERT(b->checkCausality());
    BOOST_ASSERT(c->checkCausality());
    BOOST_ASSERT(d->checkCausality());
    BOOST_ASSERT(u->checkCausality());

    return u;
}

Vertex* Simulation::doMove(Vertex* a, Vertex* b, MOVES move) {
    switch (move) {
        case COLLAPSE:
            return doCollapse(a, b);
        case FLIP:
            return doFlip(a, b);
        case ALEXANDER:
            return doAlexander(a, b);
    };

    return NULL;
}

Triangle* Simulation::generateInitialTriangulation(int N, int T) {
    Vertex * vertices[N * T];
    Triangle * triangles[N * T * 2]; // TODO: remove, unnecessary

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

