/* 
 * File:   Vertex.h
 * Author: ben
 *
 * Created on September 19, 2012, 3:31 PM
 */

#ifndef VERTEX_H
#define	VERTEX_H

#include "Utils.h"


class Triangle;
class Vertex;
typedef boost::unordered_set<Triangle*> TriSet;
typedef boost::unordered_set<Vertex*> VertSet;

class Vertex {
public:
    Vertex();
    Vertex(const Vertex& orig);
    virtual ~Vertex();

    void registerTriangle(Triangle* t) {
        triangles.insert(t);
    }

    TriSet& getTriangles() {
        return triangles;
    }

    static void getAdjacentTriangles(Vertex* a, Vertex* b, Triangle** first, Triangle** second);

    /**
     * Gets the amount of neighbouring vertices.
     * @return 
     */
    int getNeighbouringVertexCount() {
        return triangles.size();
    }

    /**
     * Gets the neighbouring vertices of this vertex.
     * @return 
     */
    VertSet getNeighbouringVertices();

    /**
     * Checks the causality requirement of this vertex.
     * @return True if vertex is causal, else false
     */
    bool checkCausality();

    /**
     * Gets all the vertices that belong to one of the four sectors.
     * @param v The vertex that is rotated about
     * @param edge One of the neighbouring vertices of v
     * @param left Count sector on the left or right of u?
     * @param tl True if the collected sector should be timelike
     * @return 
     */
    static VertSet getSectorVertices(Vertex* v, Vertex* edge, bool left, bool tl);

private:
    TriSet triangles; // set of all triangles 
};

#endif	/* VERTEX_H */

