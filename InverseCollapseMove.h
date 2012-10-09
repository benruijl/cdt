/* 
 * File:   InverseCollapseMove.h
 * Author: bruijl
 *
 * Created on October 9, 2012, 3:22 PM
 */

#ifndef INVERSECOLLAPSEMOVE_H
#define	INVERSECOLLAPSEMOVE_H

#include "Move.h"
#include "Simulation.h"

class InverseCollapseMove : public Move {
private:
    Vertex* u, *v;
    bool isTimelike;
public:

    InverseCollapseMove(bool timelike) : Move(!timelike * 2, timelike * 2) {
        isTimelike = timelike;
    }

    double getTransitionProbability(VertSet& vertices) {
        return 1.0 / (vertices.size() * u->getNeighbouringVertexCount()) +
                1.0 / (vertices.size() * v->getNeighbouringVertexCount());
    }

    bool isMovePossible(VertSet& vertices) {
        return true;
    }

    Move* generateRandomMove(Simulation& simulation) {
        u = simulation.getRandomVertex(simulation.getVertices());
        v = simulation.getRandomVertex(u->getNeighbouringVertices());
        return this;
    }

    double getInverseTransitionProbability(VertSet& vertices) {
        // TODO
    }

    void execute(VertSet& vertices) {
        /*Triangle* first, *second;
        Vertex::getAdjacentTriangles(a, b, &first, &second);

        bool lAB = first->isTimelike(a, b);
        Vertex::getAdjacentTriangles(b, c, &first, &second);
        bool lBC = first->isTimelike(b, c); // not really required

        // Collected all triangles on one side of the barrier a - b - c
        TriSet tri;
        Triangle* cur = first;
        Vertex* edgeVertex = cur->getThirdVertex(b, c);

        do {
            tri.insert(cur);
            Vertex::getAdjacentTriangles(edgeVertex, b, &first, &second);
            cur = first == cur ? second : first;
            edgeVertex = cur->getThirdVertex(edgeVertex, b);
        } while (edgeVertex != a); // TODO: check if it collects all

        // Create new vertex
        b->getTriangles() -= tri;
        Vertex* u = new Vertex();
        u->getTriangles() += tri;

        foreach(Triangle* t, tri) {
            t->replaceVertex(b, u);
        }

        // Create two new triangles
        new Triangle(a, u, b, lAB, !lAB, lAB);
        new Triangle(c, u, b, lAB, !lAB, lAB);
        vertices.insert(u);

        BOOST_ASSERT(b->checkCausality());
        BOOST_ASSERT(u->checkCausality());
        */
    }
};


#endif	/* INVERSECOLLAPSEMOVE_H */

