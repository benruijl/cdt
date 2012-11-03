/* 
 * File:   InversePinchingMove.h
 * Author: bruijl
 *
 * Created on October 15, 2012, 1:33 PM
 */

#ifndef INVERSEPINCHINGMOVE_H
#define	INVERSEPINCHINGMOVE_H

#include "Move.h"

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

    double getTransitionProbability(std::vector<Vertex*>& vertices) {
        Triangle* start = *v->getTriangles().begin();
        TriSet triRight = v->getSectorTriangles(start, false, false);
        TriSet triLeft = v->getSectorTriangles(start, true, false);

        return 1.0 / (vertices.size() * triRight.size() * triLeft.size() * 2);
    }

    bool isMovePossible(std::vector<Vertex*>& vertices) {
        return t != NULL && s != NULL;
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

    double getInverseTransitionProbability(std::vector<Vertex*>& vertices) {
        /* TODO: repetition of code, same thing done in execute as well*/
        Vertex* u = t->getNextVertex(v);
        Vertex* y = s->getNextVertex(v);
        Vertex* z = s->getNextVertex(y);

        if (left) {
            u = t->getNextVertex(u);
        }

        int count = 1; // last vertex is not counted, so add it here
        Triangle* cur = t;
        Vertex* edge = u;

        while (edge != y && edge != z) {
            cur = cur->getNeighbour(v, edge);
            edge = cur->getThirdVertex(v, edge);
            count++;
        }

        // count + 1 because there is a new vertex
        return 1.0 / ((vertices.size() + 1) * (count + 2));
    }

    void execute(std::vector<Vertex*>& vertices) {
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

        t->removeVertices();
        s->removeVertices();
        delete t;
        delete s;

        Vertex* x = new Vertex();
        vertices.push_back(x);

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

        BOOST_ASSERT(v->checkCausality());
        BOOST_ASSERT(x->checkCausality());

    }

    std::string printID() {
        return std::string("INV_PINCH");
    }
};

#endif	/* INVERSEPINCHINGMOVE_H */

