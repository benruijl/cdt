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
     * 
     * TODO: rewrite in terms of sector vertices
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

        while (edgeVertex != w) {
            Vertex::getAdjacentTriangles(edgeVertex, u, &first, &second);
            cur = first == cur ? second : first;
            edgeVertex = cur->getThirdVertex(edgeVertex, u);
            tri.insert(cur);
        }

        return tri;
    }

    VertSet getLinksOfSameType(bool left) {
        Triangle* first, *second;
        Vertex::getAdjacentTriangles(u, v, &first, &second);
        bool tl = first->isTimelike(u, v);

        VertSet vert;
        Triangle* cur = left ? second : first;
        Vertex* edgeVertex = v;

        while (cur->isTimelike(u, edgeVertex) == tl) {
            vert.insert(edgeVertex);
            Vertex::getAdjacentTriangles(edgeVertex, u, &first, &second);
            cur = first == cur ? second : first;
            edgeVertex = cur->getThirdVertex(edgeVertex, u);
        }

        return vert;
    }

    /**
     * Find all the vertices w of which the link u-w belongs to the same sector as the
     * link u - v.
     * @return 
     */
    VertSet getSectorVertices() {
        VertSet v = getLinksOfSameType(true);
        v += getLinksOfSameType(false);
        return v;
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

        VertSet sector = u->getSectorVertices(first, true, !isTimelike);

       // std::cout << (lUV == lUW) << " " << (lUV != isTimelike) << " " <<
         //       (sector.find(v) == sector.end()) << " " << (sector.find(w) == sector.end()) << std::endl;
        return (lUV == lUW) && (lUV != isTimelike) &&
                ((sector.find(v) == sector.end()) != (sector.find(w) == sector.end()));
    }

    Move * generateRandomMove(Simulation & simulation) {
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

        BOOST_ASSERT(u->checkCausality());
        BOOST_ASSERT(v->checkCausality());
        BOOST_ASSERT(w->checkCausality());
        BOOST_ASSERT(x->checkCausality());
    }

    std::string printID() {
        return std::string("INV_COLL_") + (isTimelike ? "TL" : "SL");
    }
};


#endif	/* INVERSECOLLAPSEMOVE_H */

