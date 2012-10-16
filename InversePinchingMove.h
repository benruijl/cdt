/* 
 * File:   InversePinchingMove.h
 * Author: bruijl
 *
 * Created on October 15, 2012, 1:33 PM
 */

#ifndef INVERSEPINCHINGMOVE_H
#define	INVERSEPINCHINGMOVE_H

#include "Vertex.h"
#include "Triangle.h"
#include "Simulation.h"

class InversePinchingMove : public Move {
private:
    Vertex* v;
    Triangle *t, *s;
    bool left; // orientation of the asymmetric expansion
public:

    InversePinchingMove() : Move(4, -2) {
        t = NULL;
        s = NULL;
    }

    double getTransitionProbability(VertSet& vertices) {
        return 1.0; // TODO
    }

    bool isMovePossible(VertSet& vertices) {
        if (t == NULL || s == NULL) {
            return false;
        }

        // TODO: this should always be the case for sector triangles, remove?
        if (t->checkAdjacentSides(v) || t->isTimelike(v) ||
                s->checkAdjacentSides(v) || s->isTimelike(v)) {
            return false;
        }

        return true;
    }

    Move* generateRandomMove(Simulation& simulation) {
        v = simulation.getRandomVertex(simulation.getVertices());
        Vertex* u = *v->getNeighbouringVertices().begin();

        TriSet tri = Triangle::getSectorTriangles(v, u, false, false);
        t = simulation.getRandomElementFromSet(tri);
        tri = Triangle::getSectorTriangles(v, u, true, false);
        s = simulation.getRandomElementFromSet(tri);

        left = simulation.getRandomNumber() < 0.5;
        return this;
    }

    double getInverseTransitionProbability(VertSet& vertices) {
        return 1.0; // TODO
    }

    void execute(VertSet& vertices) {
        Vertex* u = t->getNextVertex(v);
        Vertex* y = s->getNextVertex(v);
        Vertex* z = s->getNextVertex(y);

        if (left) {
            u = t->getNextVertex(u);
        }

        TriSet tri;
        Triangle* cur = t, *q, *r;
        Vertex* edge = u;
        // find the vertex which comes first
        while (edge != y && edge != z) {
            tri.insert(cur);
            Vertex::getAdjacentTriangles(v, edge, &q, &r);
            cur = cur == q ? r : q;
            edge = cur->getThirdVertex(u, edge);
        }

        tri.insert(s);

        // the three vertices are now u -- v -- edge
        // TODO: test extensively
        Vertex* x = new Vertex();
        vertices.insert(x);

        x->getTriangles().insert(v->getTriangles());
        x->getTriangles() -= tri;
        v->getTriangles() -= x->getTriangles();

        foreach(Triangle* a, x->getTriangles()) {
            a->replaceVertex(v, x);
        }

        new Triangle(Triangle::TTS, u, x, v);
        new Triangle(Triangle::TTS, v, x, edge);

        BOOST_ASSERT(v->checkCausality());
        BOOST_ASSERT(x->checkCausality());

    }
};

#endif	/* INVERSEPINCHINGMOVE_H */

