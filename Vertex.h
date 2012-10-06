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
     * Checks the causality requirement of this vertex.
     * @return True if vertex is causal, else false
     */
    bool checkCausality();

private:
    TriSet triangles; // set of all triangles 
};

#endif	/* VERTEX_H */

