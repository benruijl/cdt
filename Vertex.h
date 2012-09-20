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

    Vertex(double x, double t) {
        this->x = x;
        this->t = t;
    }

    bool operator==(const Vertex &v) {
        return x == v.x && t == v.t;
    }

    void registerTriangle(Triangle* t) {
        if (triangle == 0) {
            triangle = t;
        }
    }

    Triangle* getTriangle() {
        return triangle;
    }

    double getX() {
        return x;
    }

    /**
     * Checks the causality requirement of this vertex.
     * @return True if vertex is causal, else false
     */
    bool checkCausality();

private:
    // FIXME: are coordinates necessary?
    double x; // position on spatial axis
    double t; // position on time axis
    Triangle* triangle; // a triangle that contains this vertex
};

#endif	/* VERTEX_H */

