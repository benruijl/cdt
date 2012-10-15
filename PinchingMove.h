/* 
 * File:   PinchingMove.h
 * Author: bruijl
 *
 * Created on October 15, 2012, 10:45 AM
 */

#ifndef PINCHINGMOVE_H
#define	PINCHINGMOVE_H

#include "Vertex.h"
#include "Triangle.h"

class PinchingMove : public Move {
private:
    Vertex *u, *v;
public:

    PinchingMove() : Move(-4, 2) {
    }

    double getTransitionProbability(VertSet& vertices) {
        // Note that this move is not symmetric, (u,v) and (v, u) are different moves.
        return 1.0 / (vertices.size() * u->getNeighbouringVertexCount());
    }

    bool isMovePossible(VertSet& vertices) {
        Triangle *first, *second, *r, *t;
        Vertex::getAdjacentTriangles(u, v, &first, &second);
        Vertex* w = first->getThirdVertex(u, v);
        Vertex* x = second->getThirdVertex(u, v);

        if (!first->isTimelike(u, v) || first->isTimelike(u, w) || second->isTimelike(u, x)
                || !first->isTimelike(v, w) || !second->isTimelike(v, x)) {
            return false;
        }

        Vertex::getAdjacentTriangles(v, w, &r, &t);
        r = r == first ? t : r;
        Vertex* y = r->getThirdVertex(v, w);

        if (r->isTimelike(v, y) || !r->isTimelike(w, y)) {
            return false;
        }

        Vertex::getAdjacentTriangles(v, x, &r, &t);
        r = r == second ? t : r;
        Vertex* z = r->getThirdVertex(v, x);

        if (r->isTimelike(v, z) || !r->isTimelike(x, z)) {
            return false;
        }

        return true;
    }

    Move* generateRandomMove(Simulation& simulation) {
        u = simulation.getRandomVertex(simulation.getVertices());
        v = simulation.getRandomVertex(u->getNeighbouringVertices());
        return this;
    }

    double getInverseTransitionProbability(VertSet& vertices) {
        return 1.0; // TODO
    }

    void execute(VertSet& vertices) {
        Triangle *first, *second, *third, *fourth, *r, *t;
        Vertex::getAdjacentTriangles(u, v, &first, &second);
        Vertex* w = first->getThirdVertex(u, v);
        Vertex* x = second->getThirdVertex(u, v);

        Vertex::getAdjacentTriangles(v, w, &r, &t);
        third = r == first ? t : r;
        Vertex* y = third->getThirdVertex(v, w);

        Vertex::getAdjacentTriangles(v, x, &r, &t);
        fourth = r == second ? t : r;
        Vertex* z = fourth->getThirdVertex(v, x);

        /* Clean-up */
        first->removeVertices();
        second->removeVertices();
        third->removeVertices();
        fourth->removeVertices();
        vertices.erase(v);

        /* Relabel */
        foreach(Triangle* t, v->getTriangles()) {
            t->replaceVertex(v, u);
            u->getTriangles().insert(t);
        }

        /* Create new triangles */
        new Triangle(w, u, y, false, false, true);
        new Triangle(x, u, z, false, false, true);

        delete first;
        delete second;
        delete third;
        delete fourth;
        delete v;

        BOOST_ASSERT(u->checkCausality());
        BOOST_ASSERT(w->checkCausality());
        BOOST_ASSERT(x->checkCausality());
        BOOST_ASSERT(y->checkCausality());
    }
};

#endif	/* PINCHINGMOVE_H */

