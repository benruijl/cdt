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

    /**
     * Check if the moves does not cause two links to overlap.
     * @return True if move is valid
     */
    bool doTopologyCheck() {
        // TODO: improve code
        Triangle *first, *second, *third, *fourth, *t, *r;
        Vertex* v = *u->getNeighbouringVertices().begin();
        Vertex::getAdjacentTriangles(u, v, &first, &second);
        Vertex* w = first->getThirdVertex(u, v);
        Vertex::getAdjacentTriangles(u, w, &t, &r);
        third = t == first ? r : t;
        Vertex* x = third->getThirdVertex(u, w);
        Vertex::getAdjacentTriangles(u, x, &t, &r);
        fourth = t == third ? r : t;
        Vertex* y = fourth->getThirdVertex(u, x);

        if (first->isTimelike(u, v) != isTimelike) {
            v = w;
            x = y;
        }

        VertSet verts = v->getNeighbouringVertices();

        return verts.find(x) == verts.end();
    }
public:

    InverseAlexanderMove(bool timelike) : Move(!timelike * -2, timelike * -2) {
        isTimelike = timelike;
    }

    double getTransitionProbability(std::vector<Vertex*>& vertices) {
        return 1.0 / vertices.size();
    }

    bool isMovePossible(std::vector<Vertex*>& vertices) {
        // vertex needs to have 4 links: two spacelike and two timelike
        if (u->getTriangles().size() != 4) {
            return false;
        }

        if (!doTopologyCheck()) {
            return false;
        }

        int tlCount = 0;

        foreach(Triangle* t, u->getTriangles()) {
            tlCount += t->isOppositeLinkTimelike(u);
        }
        
        return tlCount ==4; // fixme

        if (tlCount != 2) {
            return (isTimelike && tlCount <= 2) || (!isTimelike && tlCount >= 2);

        }

        // The check for tlCount == 2 is more complicated
        // TODO: find a better way
        Triangle *first, *second, *third, *fourth, *t, *r;
        Vertex* v = *u->getNeighbouringVertices().begin();
        Vertex::getAdjacentTriangles(u, v, &first, &second);
        Vertex* w = first->getThirdVertex(u, v);
        Vertex::getAdjacentTriangles(u, w, &t, &r);
        third = t == first ? r : t;
        Vertex* x = third->getThirdVertex(u, w);
        Vertex::getAdjacentTriangles(u, x, &t, &r);
        fourth = t == third ? r : t;
        Vertex* y = fourth->getThirdVertex(u, x);

        bool lUV = first->isTimelike(u, v);
        bool lVW = first->isTimelike(v, w);
        bool lWX = third->isTimelike(w, x);
        bool lXU = third->isTimelike(x, u);
        bool lXY = fourth->isTimelike(x, y);
        bool lYV = second->isTimelike(y, v);
        bool lYU = second->isTimelike(y, u);
        bool lUW = first->isTimelike(u, w);

        if (first->isOppositeLinkTimelike(u) != second->isOppositeLinkTimelike(u) &&
                first->isOppositeLinkTimelike(u) != third->isOppositeLinkTimelike(u)) {
            return true; //symmetric configuration
        }

        if (lUV == lVW && lVW == lWX && lWX == lXU) return first->isTimelike(u, v) != isTimelike;
        if (lUW == lWX && lWX == lXY && lXY == lYU) return first->isTimelike(u, w) != isTimelike;
        if (lXY == lYV && lYV == lUV && lUV == lXU) return first->isTimelike(u, v) != isTimelike;
        if (lUW == lVW && lVW == lYV && lYV == lYU) return first->isTimelike(u, w) != isTimelike;

        BOOST_ASSERT(false);
    }

    Move* generateRandomMove(Simulation& simulation) {
        u = simulation.getRandomVertex(simulation.getVertices());
        return this;
    }

    double getInverseTransitionProbability(std::vector<Vertex*>& vertices) {
        Triangle *first, *second;
        double prob = 0;

        foreach(Vertex* v, u->getNeighbouringVertices()) {
            Vertex::getAdjacentTriangles(u, v, &first, &second);

            if (first->isTimelike(u, v) == isTimelike) {
                // -1 because vertex u does not exist
                prob += 1.0 / ((vertices.size() - 1) *
                        (v->getNeighbouringVertexCount()));
            }
        }

        return prob;
    }

    void execute(std::vector<Vertex*>& vertices) {
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
        vertices.erase(std::remove(vertices.begin(), vertices.end(), u), vertices.end());
        first->removeVertices();
        second->removeVertices();
        third->removeVertices();
        fourth->removeVertices();
        delete first;
        delete second;
        delete third;
        delete fourth;
        delete u;

        if (lUV != isTimelike) {
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

    std::string printID() {
        return std::string("INV_ALEX_") + (isTimelike ? "TL" : "SL");
    }
};

#endif	/* INVERSEALEXANERMOVE_H */
