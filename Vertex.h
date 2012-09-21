/* 
 * File:   Vertex.h
 * Author: ben
 *
 * Created on September 19, 2012, 3:31 PM
 */

#ifndef VERTEX_H
#define	VERTEX_H

class Triangle;

class Vertex {
public:
    Vertex();
    Vertex(const Vertex& orig);
    virtual ~Vertex();

    void registerTriangle(Triangle* t) {
        if (triangle == 0) {
            triangle = t;
        }
    }

    Triangle* getTriangle() {
        return triangle;
    }

    /**
     * Checks the causality requirement of this vertex.
     * @return True if vertex is causal, else false
     */
    bool checkCausality();

private:
    Triangle* triangle; // a triangle that contains this vertex
};

#endif	/* VERTEX_H */

