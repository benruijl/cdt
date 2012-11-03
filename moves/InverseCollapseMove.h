/* 
 * File:   InverseCollapseMove.h
 * Author: bruijl
 *
 * Created on October 9, 2012, 3:22 PM
 */

#ifndef INVERSECOLLAPSEMOVE_H
#define	INVERSECOLLAPSEMOVE_H

#include "Move.h"

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
    TriSet getBarrierTriangles(bool left) {
        Triangle* first, *second;
        Vertex::getAdjacentTriangles(u, v, &first, &second);

        // Collected all triangles on one side of the barrier v - u - w
        TriSet tri;
        Triangle* cur = left ? second : first;
        Vertex* edgeVertex = v;

        while (edgeVertex != w) {
            cur = cur->getNeighbour(u, edgeVertex);
            edgeVertex = cur->getThirdVertex(u, edgeVertex);
            tri.insert(cur);
        }

        return tri;
    }
public:

    InverseCollapseMove(bool timelike) : Move(!timelike * 2, timelike * 2) {
        isTimelike = timelike;
    }

    double getTransitionProbability(std::vector<Vertex*>& vertices) {
        return 1.0 / (vertices.size() * u->getNeighbouringVertexCount() *
                (u->getNeighbouringVertexCount() - 1) / 2.0);
    }

    bool isMovePossible(std::vector<Vertex*>& vertices) {
        Triangle* first, *second;
        Vertex::getAdjacentTriangles(u, v, &first, &second);
        bool lUV = first->isTimelike(u, v);
        Vertex::getAdjacentTriangles(u, w, &first, &second);
        bool lUW = first->isTimelike(u, w);

        if (lUV != lUW || lUV == isTimelike) {
            return false;
        }

        // the vertices should belong to different sectors
        VertSet sector = u->getSectorVertices(first, true, !isTimelike);

        return (sector.find(v) == sector.end()) != (sector.find(w) == sector.end());
    }

    Move* generateRandomMove(Simulation& simulation) {
        u = simulation.getRandomVertex(simulation.getVertices());
        VertSet neighbours = u->getNeighbouringVertices();
        v = simulation.getRandomElementFromSet(neighbours);
        neighbours.erase(v);
        w = simulation.getRandomElementFromSet(neighbours);

        return this;
    }

    double getInverseTransitionProbability(std::vector<Vertex*>& vertices) {
        int numTriLeft = getBarrierTriangles(true).size();

        // the probability of the inverse is selecting a vertex and then one of their neighbours.
        // The number of neighbours depends on the vertex and is calculated in numTriLeft.
        return 1.0 / ((vertices.size() + 1) * (numTriLeft + 2)) +
                1.0 / ((vertices.size() + 1) *
                (u->getTriangles().size() - numTriLeft + 2));
    }

    void execute(std::vector<Vertex*>& vertices) {
        Triangle* first, *second;
        Vertex::getAdjacentTriangles(u, v, &first, &second);
        bool lUV = first->isTimelike(u, v);

        // Collected all triangles on one side of the barrier v - u - w
        TriSet tri = getBarrierTriangles(true);

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

        vertices.push_back(x);

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

