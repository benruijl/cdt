/* 
 * File:   AlexanderMove.h
 * Author: bruijl
 *
 * Created on October 9, 2012, 1:37 PM
 */

#ifndef ALEXANDERMOVE_H
#define	ALEXANDERMOVE_H

#include "Move.h"

class AlexanderMove : public Move {
private:
    Vertex* u, *v;
    bool isTimelike;
public:

    AlexanderMove(bool timelike) : Move(!timelike * 2, timelike * 2) {
        isTimelike = timelike;
    }

    double getTransitionProbability(std::vector<Vertex*>& vertices) {
        return 1.0 / (vertices.size() * u->getNeighbouringVertexCount()) +
                1.0 / (vertices.size() * v->getNeighbouringVertexCount());
    }

    bool isMovePossible(std::vector<Vertex*>& vertices) {
        Triangle* first, *second;
        Vertex::getAdjacentTriangles(u, v, &first, &second);

        return isTimelike == first->isTimelike(u, v);
    }

    Move* generateRandomMove(Simulation& simulation) {
        u = simulation.getRandomVertex(simulation.getVertices());
        v = simulation.getRandomElementFromSet(u->getNeighbouringVertices());
        return this;
    }

    double getInverseTransitionProbability(std::vector<Vertex*>& vertices) {
        // Alexander moves create one new vertex and the inverse needs
        // only this vertex to perform the move
        return 1.0 / (vertices.size() + 1);
    }

    void execute(std::vector<Vertex*>& vertices) {
        Triangle* first, *second;
        Vertex::getAdjacentTriangles(u, v, &first, &second);

        Vertex* c = first->getThirdVertex(u, v);
        Vertex* d = second->getThirdVertex(u, v);

        /* Get link types */
        bool lAB = first->isTimelike(u, v);
        bool lAC = first->isTimelike(u, c);
        bool lCB = first->isTimelike(c, v);
        bool lAD = second->isTimelike(u, d);
        bool lBD = second->isTimelike(v, d);
        bool newLink = !lAB;

        Vertex* e = new Vertex(); // new vertex
        new Triangle(u, c, e, lAC, newLink, lAB);
        new Triangle(e, c, v, newLink, lCB, lAB);
        new Triangle(u, e, d, lAB, newLink, lAD);
        new Triangle(e, v, d, lAB, lBD, newLink);

        vertices.push_back(e);
        first->removeVertices();
        second->removeVertices();
        delete first;
        delete second;

        BOOST_ASSERT(u->checkCausality());
        BOOST_ASSERT(v->checkCausality());
        BOOST_ASSERT(c->checkCausality());
        BOOST_ASSERT(d->checkCausality());
        BOOST_ASSERT(e->checkCausality());
    }

    std::string printID() {
        return std::string("ALEX_") + (isTimelike ? "TL" : "SL");
    }
};

#endif	/* ALEXANDERMOVE_H */

