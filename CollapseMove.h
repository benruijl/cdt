/* 
 * File:   CollapseMove.h
 * Author: bruijl
 *
 * Created on October 9, 2012, 2:24 PM
 */

#ifndef COLLAPSEMOVE_H
#define	COLLAPSEMOVE_H

#include "Move.h"
#include "Utils.h"

class CollapseMove : public Move {
private:
    Vertex* u, *v;
    bool isTimelike;

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

        if (first->isTimelike(u, v) != isTimelike || !first->checkAdjacentSides(u, v)
                || !second->checkAdjacentSides(u, v)) {
            return false;
        }

        // topology constraint: because of periodic boundary conditions it could
        // be that the collapse move fails and results in overlapping links
        // TODO find cleaner way
        Vertex x; // new test vector
        x.getTriangles() += u->getTriangles();
        x.getTriangles().erase(first);
        x.getTriangles().erase(second);

        Vertex y; // new test vector
        y.getTriangles() += v->getTriangles();
        y.getTriangles().erase(first);
        y.getTriangles().erase(second);

        VertSet verts = y.getNeighbouringVertices();
        verts = verts & x.getNeighbouringVertices();

        // the third vertex of first and second are always there
        return verts.size() == 2;
    }

    Move* generateRandomMove(Simulation& simulation) {
        u = simulation.getRandomVertex(simulation.getVertices());
        v = simulation.getRandomVertex(u->getNeighbouringVertices());

        return this;
    }

    double getInverseTransitionProbability(VertSet& vertices) {
        // the inverse move selects two vertices from the surroundings of a
        // chosen central vertex.
        VertSet neighbours = u->getNeighbouringVertices();
        neighbours += v->getNeighbouringVertices();

        // -3 because the vertices should not belong to the same triangles
        return 1.0 / ((vertices.size() + 1) * neighbours.size() *
                (neighbours.size() - 3) / 2.0);
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

    std::string printID() {
        return std::string("COLL_") + (isTimelike ? "TL" : "SL");
    }
};

#endif	/* COLLAPSEMOVE_H */

