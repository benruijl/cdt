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
        neighbours[0] = NULL;
        neighbours[1] = NULL;
        neighbours[2] = NULL;

        a->registerTriangle(this);
        b->registerTriangle(this);
        c->registerTriangle(this);
    }

    /**
     * (Inefficient) helper method to click together triangles.
     * @param a First triangle
     * @param b Second triangle
     */
    static void registerNeighbour(Triangle* a, Triangle* b) {
        bool first = true;
        int u, v, w, y;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (a->vertices[i] == b->vertices[j]) {
                    if (first) {
                        u = i;
                        v = j;
                        first = false;
                    } else {
                        w = i;
                        y = j;
                    }

                    break;
                }
            }
        }

        if (u + w == 1) {
            a->neighbours[0] = b;
        } else if (u + w == 2) {
            a->neighbours[2] = b;
        } else {
            a->neighbours[1] = b;
        }

        if (v + y == 1) {
            b->neighbours[0] = a;
        } else if (v + y == 2) {
            b->neighbours[2] = a;
        } else {
            b->neighbours[1] = a;
        }
    }

    void registerNeighbour(int p, Triangle* neighbour) {
        neighbours[p] = neighbour;
    }


	/**
	  Sets the new neighbour for one triangle.
	  */
	void replaceNeighbour(Triangle* old, Triangle* n) {
		for (int i = 0; i < 3; i++) {
			if (neighbours[i] == old) {
				neighbours[i] = n;
				return;
			}
		}
	}

	int getLink(Vertex* a, Vertex* b) {
		int i = indexFromVertex(a);
		if (vertices[(i + 1) % 3] == b) {
			return i;
		}

		return (i - 1) % 3;
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

        return -1;
    }

	Vertex* getVertex(int index) {
		return vertices[index];
	}

	Triangle* getNeighbour(int index) {
		return neighbours[index];
	}

    Triangle* getNeighbourClockwise(Vertex* v) {
        int i = (indexFromVertex(v) +2) % 3;
        return neighbours[i];
    }

    Triangle** getNeighboursAdjecentToVertex(Vertex* v) {
        Triangle** n = new Triangle*[2];
        if (vertices[0] == v) {
            n[0] = neighbours[0];
            n[1] = neighbours[2];
        } else if (vertices[1] == v) {
            n[0] = neighbours[0];
            n[1] = neighbours[1];
        } else {
            n[0] = neighbours[1];
            n[1] = neighbours[2];
        }

        return n;
    }

    /**
     * Get the three triangles that this triangle shares edges with.
     * @return Vector of triangles
     */
    Triangle** getNeighbours() {
        return neighbours;
    }

private:
    TYPE type;
    Vertex* vertices[3]; /* Each triangle has three vertices */
    /* Each triangle has max. three neighbours. Neighbour 0 is contains vertex 0 and 1, etc. */
    Triangle* neighbours[3];
};

#endif	/* TRIANGLE_H */

