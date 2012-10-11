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
    Vertex* u, *v, *w; // u is the center triangle
    bool isTimelike;

    /**
     * Gets all the triangles that contain vertex u and that are on either the left
     * or right side of the barrier v - u -w.
     * @param left Check the left side?
     * @return 
     */
    TriSet getNeighbouringTriangles(bool left) {
        Triangle* first, *second;
        Vertex::getAdjacentTriangles(u, v, &first, &second);

        // Collected all triangles on one side of the barrier v - u - w
        TriSet tri;
        Triangle* cur = left ? second : first;
        Vertex* edgeVertex = v;

        do {
            Vertex::getAdjacentTriangles(edgeVertex, u, &first, &second);
            cur = first == cur ? second : first;
            edgeVertex = cur->getThirdVertex(edgeVertex, u);
            tri.insert(cur);
        } while (edgeVertex != w); // TODO: check if it collects all

        return tri;
    }
public:

    InverseCollapseMove(bool timelike) : Move(!timelike * 2, timelike * 2) {
        isTimelike = timelike;
    }

    double getTransitionProbability(VertSet& vertices) {
        // -2 because the third vertex must not belong to the same triangle
        return 1.0 / (vertices.size() * u->getNeighbouringVertexCount() *
                (u->getNeighbouringVertexCount() - 2) / 2.0);
    }

    bool isMovePossible(VertSet& vertices) {
        Triangle* first, *second;
        Vertex::getAdjacentTriangles(u, v, &first, &second);
        bool lUV = first->isTimelike(u, v);
        Vertex::getAdjacentTriangles(u, w, &first, &second);
        bool lUW = first->isTimelike(u, w);

        return lUV == lUW && lUV != isTimelike;
    }

    Move* generateRandomMove(Simulation& simulation) {
        u = simulation.getRandomVertex(simulation.getVertices());
        v = simulation.getRandomVertex(u->getNeighbouringVertices());

        Triangle* first, *second;
        Vertex::getAdjacentTriangles(u, v, &first, &second);

        // select a third neighbouring vertex that is not v
        // the triangle should also not belong to the same triangle as u and v
        w = v;
        while (w == v || w == first->getThirdVertex(u, v) ||
                w == second->getThirdVertex(u, v)) {
            w = simulation.getRandomVertex(u->getNeighbouringVertices());
        }

        return this;
    }

    double getInverseTransitionProbability(VertSet& vertices) {
        int numTriLeft = getNeighbouringTriangles(true).size();

        // the probability of the inverse is selecting a vertex and then one of their neighbours.
        // The number of neighbours depends on the vertex and is calculated in numTriLeft.
        return 1.0 / ((vertices.size() - 1) * numTriLeft) +
                1.0 / ((vertices.size() - 1) * (u->getTriangles().size() - numTriLeft));
    }

    void execute(VertSet& vertices) {
        Triangle* first, *second;
        Vertex::getAdjacentTriangles(u, v, &first, &second);
        bool lUV = first->isTimelike(u, v);

        // Collected all triangles on one side of the barrier v - u - w
        TriSet tri = getNeighbouringTriangles(true);
        std::cout << "tl " << isTimelike << std::endl;
        std::cout << tri.size() << " ";
        std::cout << u->getTriangles().size() << " ";

        // Create new vertex
        Vertex* x = new Vertex();
        u->getTriangles() -= tri;
        x->getTriangles() += tri;

        foreach(Triangle* t, tri) {
            t->replaceVertex(u, x);
        }

        // Create two new triangles
        new Triangle(v, x, u, lUV, !lUV, lUV);
        new Triangle(w, x, u, lUV, !lUV, lUV);

        vertices.insert(x);

        std::cout << u->getTriangles().size() << " " << x->getTriangles().size() << std::endl;

        Simulation s;
        TriSet tri2;
        s.collectTriangles(tri2, *vertices.begin(), 1);
        s.drawPartialTriangulation("graph.dot", *vertices.begin(), tri2);

        BOOST_ASSERT(u->checkCausality());
        BOOST_ASSERT(v->checkCausality());
        BOOST_ASSERT(w->checkCausality());
        BOOST_ASSERT(x->checkCausality());
        std::cout << "Causality check done " << std::endl;
    }
};


#endif	/* INVERSECOLLAPSEMOVE_H */

