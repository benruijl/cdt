/* 
 * File:   InverseAlexanerMove.h
 * Author: bruijl
 *
 * Created on October 9, 2012, 3:08 PM
 */

#ifndef INVERSEALEXANDERMOVE_H
#define	INVERSEALEXANDERMOVE_H

#include "Move.h"


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
        // FIXME: also check if the isTimelike matches the environment
        return u->getTriangles().size() == 4;
    }

    Move* generateRandomMove(Simulation& simulation) {
        u = simulation.getRandomVertex(simulation.getVertices());
        return this;
    }

    double getInverseTransitionProbability(VertSet& vertices) {
        // FIXME: this is hard
        return 1;
    }

    void execute(VertSet& vertices) {
        // TODO: different from previous version
        
     /*   Triangle* first, *second, *third, *fourth;
        Vertex::getAdjacentTriangles(a, u, &first, &second);

        Vertex* c = first->getThirdVertex(a, u);
        Vertex* d = second->getThirdVertex(a, u);

        vertices.erase(u);
        vertices.insert(c);
        vertices.insert(d);

        // Get link types
        bool lAB = first->isTimelike(a, u);
        bool lAC = first->isTimelike(a, c);
        bool lAD = second->isTimelike(a, d);

        Vertex::getAdjacentTriangles(u, b, &third, &fourth);

        bool lCB, lBD;
        if (third->containsVertex(c)) { // order could be turned around, so check!
            lCB = third->isTimelike(b, c);
            lBD = fourth->isTimelike(b, d);
        } else {
            lBD = third->isTimelike(b, d);
            lCB = fourth->isTimelike(b, c);
        }

        first->removeVertices();
        second->removeVertices();
        third->removeVertices();
        fourth->removeVertices();
        delete first;
        delete second;
        delete third;
        delete fourth;
        delete u;

        new Triangle(a, c, b, lAC, lCB, lAB);
        new Triangle(a, b, d, lAB, lBD, lAD);

        BOOST_ASSERT(a->checkCausality());
        BOOST_ASSERT(b->checkCausality());
        BOOST_ASSERT(c->checkCausality());
        BOOST_ASSERT(d->checkCausality());

        return a;*/
    }
};

#endif	/* INVERSEALEXANERMOVE_H */

