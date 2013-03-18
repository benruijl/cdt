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

typedef boost::unordered_set<Vertex*> VertSet;
typedef boost::container::flat_set<Triangle*> TriSet;

class Vertex {
public:
    Vertex();
    Vertex(const Vertex& orig);
    virtual ~Vertex();

    /**
     * Register that a triangle is using this vertex.
     * @param t Triangle to register
     */
    void registerTriangle(Triangle* t) {
        triangles.insert(t);
    }

    TriSet& getTriangles() {
        return triangles;
    }

    /**
     * Note that the order of which the first and second triangle are returned
     * should be considered random.
     * @param a
     * @param b
     * @param first
     * @param second
     */
    static void getAdjacentTriangles(const Vertex* a, const Vertex* b,
            Triangle** first, Triangle** second);

    /**
     * Gets the amount of neighbouring vertices.
     * @return 
     */
    int getNeighbouringVertexCount() {
        return triangles.size();
    }
    
    bool isNeighbour(Vertex* v);
    
    /**
     * Gets a single neighbouring vertex.
     * @return 
     */
    Vertex* getNeighbouringVertex();

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
     * Gets sector vertices starting from edge u of triangle start rotated
     * around this triangle.
     * @param start Triangle to start the search from
     * @param u Edge vertex to start from
     * @param tl True if the collected sector should be timelike
     * @return 
     */
    VertSet getSectorVertices(Triangle* start, Vertex* u, bool tl);

    /**
     * Gets all the vertices that belong to one of the four sectors.
     * @param start Triangle to start the search from
     * @param left Count sector on the left or right of start?
     * @param tl True if the collected sector should be timelike
     * @return 
     */
    VertSet getSectorVertices(Triangle* start, bool left, bool tl);

    /**
     * Gets the sector vertices of the same type that does not contain u
     * @param u Vertex to take the opposite sector of
     */
    VertSet getOtherSectorVertices(Vertex* u);

    /**
     * Gets all the triangles that belong to one of the four sectors. The two links
     * of the triangle should be both spacelike or timelike in this vertex.
     * @param v The vertex that is rotated about
     * @param start Triangle to start the search from
     * @param left Count sector on the left or right of u?
     * @param tl True if the collected sector should be timelike
     * @return 
     */
    TriSet getSectorTriangles(Triangle* start, bool left, bool tl);
    
    void printConnectivity();

private:
    TriSet triangles; // ordered vector of all triangles 
};

#endif	/* VERTEX_H */

