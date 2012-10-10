/* 
 * File:   CollapseMove.h
 * Author: bruijl
 *
 * Created on October 9, 2012, 2:24 PM
 */

#ifndef COLLAPSEMOVE_H
#define	COLLAPSEMOVE_H

#include "Move.h"

class CollapseMove : public Move {
private:
    Vertex* u, *v;
    bool isTimelike;

    /**
     * Counts all ways to select three vertices from the correct surroundings
     * after a move is done to collapse u and v.
     * @param u
     * @param v
     * @return 
     */
    int getInverseCollapseMoveCount(Vertex* u, Vertex* v) {
        int count = 0;

        /* Find out if collapse is spatial */
        Triangle* first, *second;
        Vertex::getAdjacentTriangles(u, v, &first, &second);
        Vertex* c = first->getThirdVertex(u, v);
        Vertex* d = second->getThirdVertex(u, v);

        bool isTimeLike = first->isTimelike(u, v);

        /* Count all relevant links */
        Triangle* cur = first;
        Vertex* edgeVertex = c;

        while (edgeVertex != d) {
            Vertex::getAdjacentTriangles(edgeVertex, u, &first, &second);
            cur = first == cur ? second : first;
            edgeVertex = cur->getThirdVertex(edgeVertex, u);

            if (cur->isTimelike(u, edgeVertex) != isTimeLike) {
                count++;
            }
        }

        /* Count all relevant links */
        Vertex::getAdjacentTriangles(u, v, &first, &second);
        cur = first;
        edgeVertex = c;

        while (edgeVertex != d) {
            Vertex::getAdjacentTriangles(edgeVertex, v, &first, &second);
            cur = first == cur ? second : first;
            edgeVertex = cur->getThirdVertex(edgeVertex, v);

            if (cur->isTimelike(v, edgeVertex) != isTimeLike) {
                count++;
            }
        }

        return (count) * (count - 1) / 2;
    }
public:

    CollapseMove(bool timelike) : Move(!timelike * -2, timelike * -2) {
        isTimelike = timelike;
    }

    double getTransitionProbability(VertSet& vertices) {
        return 1.0 / (vertices.size() * u->getNeighbouringVertexCount()) +
                1.0 / (vertices.size() * v->getNeighbouringVertexCount());
    }

    bool isMovePossible(VertSet& vertices) {
        Triangle* first, *second;
        Vertex::getAdjacentTriangles(u, v, &first, &second);

        // the same for spacelike and timelike
        return !first->isTimelike(u, v) && first->checkAdjacentSides(u, v)
                && second->checkAdjacentSides(u, v);
    }

    Move* generateRandomMove(Simulation& simulation) {
        u = simulation.getRandomVertex(simulation.getVertices());
        v = simulation.getRandomVertex(u->getNeighbouringVertices());
        return this;
    }

    double getInverseTransitionProbability(VertSet& vertices) {
        // FIXME: is this correct? Or do these yield different triangulations?
        return (double) getInverseCollapseMoveCount(u, v) / (vertices.size() + 1);
    }

    void execute(VertSet& vertices) {
        Triangle* first, *second;
        Vertex::getAdjacentTriangles(u, v, &first, &second);

        Vertex* c = first->getThirdVertex(u, v);
        Vertex* d = second->getThirdVertex(u, v);

        /* A collapse is only valid if the to be merged links are of the same type */
        BOOST_ASSERT(first->checkAdjacentSides(u, v));
        BOOST_ASSERT(second->checkAdjacentSides(u, v));

        first->removeVertices();
        second->removeVertices();

        v->getTriangles() += u->getTriangles();

        foreach(Triangle* t, u->getTriangles()) {
            t->replaceVertex(u, v);
        }

        vertices.erase(u);
        delete u;
        delete first;
        delete second;

        BOOST_ASSERT(v->checkCausality());
        BOOST_ASSERT(c->checkCausality());
        BOOST_ASSERT(d->checkCausality());
    }
};

#endif	/* COLLAPSEMOVE_H */

