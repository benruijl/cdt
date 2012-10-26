/* 
 * File:   FlipMove.h
 * Author: bruijl
 *
 * Created on October 9, 2012, 2:34 PM
 */

#ifndef FLIPMOVE_H
#define	FLIPMOVE_H

#include "Move.h"

class FlipMove : public Move {
private:
    Vertex* u, *v;
    bool isTimelike, change;
public:

    /**
     * A flip move.
     * The inverse flip move is a move with spacelike instead of timelike.
     * @param timelike Is the link timelike?
     * @param change Should the timelike link change to spacelike and vice versa?
     */
    FlipMove(bool timelike, bool change) : Move(!timelike * -2 * change, timelike * 2 * change) {
        isTimelike = timelike;
        this->change = change;
    }

    double getTransitionProbability(VertSet& vertices) {
        return 1.0 / (vertices.size() * u->getNeighbouringVertexCount()) +
                1.0 / (vertices.size() * v->getNeighbouringVertexCount());
    }

    bool isMovePossible(VertSet& vertices) {
        Triangle* first, *second;
        Vertex::getAdjacentTriangles(u, v, &first, &second);
        Vertex* c = first->getThirdVertex(u, v);
        Vertex* d = second->getThirdVertex(u, v);

        return first->isTimelike(u, v) && first->isTimelike(u, c) != second->isTimelike(u, d)
                && first->isTimelike(v, c) != second->isTimelike(v, d);
    }

    Move* generateRandomMove(Simulation& simulation) {
        u = simulation.getRandomVertex(simulation.getVertices());
        v = simulation.getRandomVertex(u->getNeighbouringVertices());
        return this;
    }

    double getInverseTransitionProbability(VertSet& vertices) {
        return 1.0 / (vertices.size() * u->getNeighbouringVertexCount()) +
                1.0 / (vertices.size() * v->getNeighbouringVertexCount());
    }

    void execute(VertSet& vertices) {
        Triangle* first, *second;
        Vertex::getAdjacentTriangles(u, v, &first, &second);

        Vertex* c = first->getThirdVertex(u, v);
        Vertex* d = second->getThirdVertex(u, v);

        /* Get link types */
        bool lNew = first->isTimelike(u, v) ^ change;
        bool lAC = first->isTimelike(u, c);
        bool lCB = first->isTimelike(c, v);
        bool lAD = second->isTimelike(u, d);
        bool lBD = second->isTimelike(v, d);

        new Triangle(u, c, d, lAC, lNew, lAD);
        new Triangle(c, v, d, lCB, lBD, lNew);

        first->removeVertices();
        second->removeVertices();
        delete first;
        delete second;

        BOOST_ASSERT(u->checkCausality());
        BOOST_ASSERT(v->checkCausality());
        BOOST_ASSERT(c->checkCausality());
        BOOST_ASSERT(d->checkCausality());
    }

    std::string printID() {
        return std::string("FLIP_") + (isTimelike ? "TL" : "SL") + (change ? "_CH" : "");
    }
};

#endif	/* FLIPMOVE_H */

