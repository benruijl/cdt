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
        Triangle* start = *v->getTriangles().begin();
        TriSet triRight = v->getSectorTriangles(start, false, false);
        TriSet triLeft = v->getSectorTriangles(start, true, false);

        return 1.0 / (vertices.size() * triRight.size() * triLeft.size() * 2);
    }

    bool isMovePossible(VertSet& vertices) {
        if (t == NULL || s == NULL) {
            return false;
        }

        // TODO: this should always be the case for sector triangles, remove?
        /*  if (t->checkAdjacentSides(v) || t->isTimelike(v, t->getNextVertex(v)) ||
                  s->checkAdjacentSides(v) || s->isTimelike(v, t->getNextVertex(v))) {
              return false;
          }*/

        return true;
    }

    Move* generateRandomMove(Simulation& simulation) {
        v = simulation.getRandomVertex(simulation.getVertices());
        Triangle* start = *v->getTriangles().begin();

        TriSet tri = v->getSectorTriangles(start, false, false);
        t = simulation.getRandomElementFromSet(tri);
        tri = v->getSectorTriangles(start, true, false);
        s = simulation.getRandomElementFromSet(tri);

        left = simulation.getRandomNumber() < 0.5;
        return this;
    }

    double getInverseTransitionProbability(VertSet& vertices) {
        return 1.0; // TODO: identify number of neighbours of v
        // return 1.0 / (vertices.size() + 1) * v->getNeighbouringVertexCount());
    }

    void execute(VertSet& vertices) {
        Vertex* u = t->getNextVertex(v);
        Vertex* y = s->getNextVertex(v);
        Vertex* z = s->getNextVertex(y);

        if (left) {
            u = t->getNextVertex(u);
        }

        TriSet tri;
        Triangle* cur = t;
        Vertex* edge = u;

        while (edge != y && edge != z) {
            cur = cur->getNeighbour(v, edge);
            edge = cur->getThirdVertex(v, edge);
            tri.insert(cur);
        }

        // the three vertices are now u -- v -- edge
        Vertex* w = t->getThirdVertex(v, u);
        z = s->getThirdVertex(v, edge);

        // remove triangles, they are SST
        if (t == s) {
            std::cout << "Fail" << std::endl;
            BOOST_ASSERT(!"fail");
        }

        t->removeVertices();
        s->removeVertices();
        delete t;
        delete s;

        Vertex* x = new Vertex();
        vertices.insert(x);

        x->getTriangles() += v->getTriangles();
        x->getTriangles() -= tri;
        v->getTriangles() -= x->getTriangles();

        foreach(Triangle* a, x->getTriangles()) {
            a->replaceVertex(v, x);
        }

        new Triangle(Triangle::TTS, u, x, v);
        new Triangle(Triangle::TTS, v, x, edge);
        new Triangle(Triangle::TTS, w, u, x);
        new Triangle(Triangle::TTS, x, edge, z);

        Simulation s;
        TriSet tri2;
        s.collectTriangles(tri2, v, 2);
        s.drawPartialTriangulation("graph.dot", v, tri2);

        BOOST_ASSERT(v->checkCausality());
        BOOST_ASSERT(x->checkCausality());

    }
};

#endif	/* INVERSEPINCHINGMOVE_H */

