/* 
 * File:   InverseAlexanerMove.h
 * Author: bruijl
 *
 * Created on October 9, 2012, 3:08 PM
 */

#ifndef INVERSEALEXANDERMOVE_H
#define	INVERSEALEXANDERMOVE_H

#include "Move.h"
#include "Triangle.h"

class InverseAlexanderMove : public Move {
private:
    Vertex* u;
    bool isTimelike;
public:

    InverseAlexanderMove(bool timelike) : Move(!timelike * -2, timelike * -2) {
        isTimelike = timelike;
    }

    double getTransitionProbability(VertSet& vertices) {
        return 1.0 / vertices.size();
    }

    bool isMovePossible(VertSet& vertices) {
        // vertex needs to have 4 links: two spacelike and two timelike
        return u->getTriangles().size() == 4;
    }

    Move* generateRandomMove(Simulation& simulation) {
        u = simulation.getRandomVertex(simulation.getVertices());
        return this;
    }

    double getInverseTransitionProbability(VertSet& vertices) {
        // TODO: find isTimelike Link and get the two vertices u and v from them
        //   return 1.0 / ((vertices.size() - 1) * u->getNeighbouringVertexCount()) +
        //           1.0 / ((vertices.size() - 1) * v->getNeighbouringVertexCount());
        return 1.0;
    }

    void execute(VertSet& vertices) {
        Triangle *first, *second, *third, *fourth, *t, *r;
        Vertex *v, *w, *x, *y;
        bool lUV, lVW, lVY, lWX, lXY;

        /* Determine the surroundings in a deterministic way */
        VertSet n = u->getNeighbouringVertices();

        v = *n.begin();
        Vertex::getAdjacentTriangles(u, v, &first, &second);
        w = first->getThirdVertex(u, v);
        y = second->getThirdVertex(u, v);

        lUV = first->isTimelike(u, v);
        lVW = first->isTimelike(v, w);
        lVY = second->isTimelike(v, y);

        Vertex::getAdjacentTriangles(u, w, &t, &r);
        third = t == first ? r : t;

        x = third->getThirdVertex(u, w);
        Vertex::getAdjacentTriangles(u, x, &t, &r);
        fourth = t == third ? r : t;

        lWX = third->isTimelike(w, x);
        lXY = fourth->isTimelike(x, y);

        /* Perform cleanup */
        vertices.erase(u);
        first->removeVertices();
        second->removeVertices();
        third->removeVertices();
        fourth->removeVertices();
        delete first;
        delete second;
        delete third;
        delete fourth;
        delete u;

        if (lUV == isTimelike) {
            new Triangle(v, w, y, lVW, !lUV, lVY);
            new Triangle(w, x, y, lWX, lXY, !lUV);
        } else {
            new Triangle(v, w, x, lVW, lWX, lUV);
            new Triangle(v, x, y, lUV, lXY, lVY);
        }


        BOOST_ASSERT(v->checkCausality());
        BOOST_ASSERT(w->checkCausality());
        BOOST_ASSERT(x->checkCausality());
        BOOST_ASSERT(y->checkCausality());
    }
};

#endif	/* INVERSEALEXANERMOVE_H */

