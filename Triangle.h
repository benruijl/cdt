/* 
 * File:   Triangle.h
 * Author: ben
 *
 * Created on September 19, 2012, 3:31 PM
 */

#ifndef TRIANGLE_H
#define	TRIANGLE_H

#include <iostream>
#include "Vertex.h"

class Triangle {
public:

    /* the two Lorentz triangles. Indices are in this order */
    enum TYPE {
        TTS, TSS
    };

    Triangle(TYPE type);
    Triangle(const Triangle& orig);
    virtual ~Triangle();

    Triangle(TYPE type, Vertex* a, Vertex* b, Vertex* c) {
        this->type = type;
        vertices[0] = a;
        vertices[1] = b;
        vertices[2] = c;

        a->registerTriangle(this);
        b->registerTriangle(this);
        c->registerTriangle(this);
    }

    int getLink(Vertex* a, Vertex* b) {
        int i = indexFromVertex(a);
        if (vertices[(i + 1) % 3] == b) {
            return i;
        }

        return (i + 2) % 3;
    }

    bool containsVertex(Vertex* a) {
        for (int i = 0; i < 3; i++) {
            if (vertices[i] == a) {
                return true;
            }
        }
        return false;
    }

    int getLightConeCount(Vertex* v) {
        int i = indexFromVertex(v);

        if (type == TTS) {
            return i != 1;
        } else {
            return i != 2;
        }
    }

    void replaceVertex(Vertex* old, Vertex* n) {
        int i = indexFromVertex(old);
        vertices[i] = n;
    }

    int indexFromVertex(Vertex* v) {
        if (vertices[0] == v) {
            return 0;
        } else if (vertices[1] == v) {
            return 1;
        } else if (vertices[2] == v) {
            return 2;
        }

        BOOST_ASSERT(false);
    }

    /**
     * Unregisters this triangles at the 3 vertices
     * @return Unregistered triangle ready for deletion
     */
    void removeVertices() {
        for (int i = 0; i < 3; i++) {
            vertices[i]->getTriangles().erase(this);
        }
    }

    Vertex* getVertex(int index) {
        return vertices[index];
    }

private:
    TYPE type;
    Vertex* vertices[3]; /* Each triangle has three vertices */
};

#endif	/* TRIANGLE_H */

