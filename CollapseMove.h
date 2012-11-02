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
#include "Simulation.h"

class CollapseMove : public Move {
private:
    Vertex* u, *v;
    bool isTimelike;

public:

    CollapseMove(bool timelike) : Move(!timelike * -2, timelike * -2) {
        isTimelike = timelike;
    }

    double getTransitionProbability(std::vector<Vertex*>& vertices) {
        return 1.0 / (vertices.size() * u->getNeighbouringVertexCount()) +
                1.0 / (vertices.size() * v->getNeighbouringVertexCount());
    }

    bool isMovePossible(std::vector<Vertex*>& vertices) {
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
        v = simulation.getRandomElementFromSet(u->getNeighbouringVertices());

        return this;
    }

    double getInverseTransitionProbability(std::vector<Vertex*>& vertices) {
        // the inverse move selects two vertices from the surroundings of a
        // chosen central vertex.
        VertSet neighbours = u->getNeighbouringVertices();
        neighbours += v->getNeighbouringVertices();

        // -2 because u and v are included in the union
        return 1.0 / ((vertices.size() - 1) * (neighbours.size() - 2) *
                (neighbours.size() - 3) / 2.0);
    }

    void execute(std::vector<Vertex*>& vertices) {
        Triangle* first, *second;
        Vertex::getAdjacentTriangles(u, v, &first, &second);

        Vertex* c = first->getThirdVertex(u, v);
        Vertex* d = second->getThirdVertex(u, v);

        first->removeVertices();
        second->removeVertices();

        v->getTriangles() += u->getTriangles();

        foreach(Triangle* t, u->getTriangles()) {
            t->replaceVertex(u, v);
        }

        // remove vertex
        vertices.erase(std::remove(vertices.begin(), vertices.end(), u), vertices.end());

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

