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

    /** 
     * The type of a triangle. To be Lorentzian, it can either have two 
     * spacelike and one timelike link or two timelike links and one spacelik. 
     * 
     * <b>Important</b>: Links should be defined in the order of the type name
     */
    enum TYPE {
        TTS, SST
    };

    Triangle(TYPE type);
    Triangle(const Triangle& orig);
    virtual ~Triangle();

    void initialize(TYPE type, Vertex* a, Vertex* b, Vertex* c) {
        this->type = type;
        vertices[0] = a;
        vertices[1] = b;
        vertices[2] = c;

        a->registerTriangle(this);
        b->registerTriangle(this);
        c->registerTriangle(this);
    }

    /**
     * Creates a new triangle. It is automatically registered with the vertices.
     * @param type Type of the triangle
     * @param a First vertex
     * @param b Second vertex
     * @param c Third vertex
     */
    Triangle(TYPE type, Vertex* a, Vertex* b, Vertex* c) {
        initialize(type, a, b, c);
    }

    /**
     * Creates a triangle, given that the links are timelike or spacelike.
     * @param a First vertex
     * @param b Second vertex
     * @param c Third vertex
     * @param timeLikeA First link is timelike
     * @param timeLikeB Second link is timelike
     * @param timeLikeC Third link is timelike
     */
    Triangle(Vertex* a, Vertex* b, Vertex* c, bool timeLikeAB, bool timeLikeBC, bool timeLikeCA) {
        if (timeLikeAB && timeLikeBC && !timeLikeCA) {
            initialize(TTS, a, b, c);
            return;
        }

        if (timeLikeBC && timeLikeCA && !timeLikeAB) {
            initialize(TTS, b, c, a);
            return;
        }

        if (timeLikeCA && timeLikeAB && !timeLikeBC) {
            initialize(TTS, c, a, b);
            return;
        }

        if (!timeLikeAB && !timeLikeBC && timeLikeCA) {
            initialize(SST, a, b, c);
            return;
        }

        if (!timeLikeBC && !timeLikeCA && timeLikeAB) {
            initialize(SST, b, c, a);
            return;
        }

        if (!timeLikeCA && !timeLikeAB && timeLikeBC) {
            initialize(SST, c, a, b);
            return;
        }

        BOOST_ASSERT(false);
    }

    int getLink(Vertex* a, Vertex* b) {
        int i = indexFromVertex(a);
        if (vertices[(i + 1) % 3] == b) {
            return i;
        }

        return (i + 2) % 3;
    }

    int getOppositeLink(Vertex* a) {
        int i = indexFromVertex(a);
        return (i + 1) % 3;
    }

    bool isOppositeLinkTimelike(Vertex* a) {
        return isTimelike(getOppositeLink(a));
    }

    /**
     * Checks if a link is timelike.
     * @param link Link number
     * @return True if timelike, false if spacelike
     */
    bool isTimelike(int link) {
        if (type == TTS) {
            return link != 2;
        }

        return link == 2;
    }

    /**
     * Returns the number of timelike links that contain vertex v.
     * @param v Vertex
     * @return 
     */
    int getAdjacentTimelikeCount(Vertex* v) {
        int i = indexFromVertex(v);
        return isTimelike(i) + isTimelike((i + 2) % 3);
    }

    /**
     * Checks if a link is timelike.
     * @param a First vertex of link 
     * @param b Second vertex of link
     * @return True if timelike, false if spacelike
     */
    bool isTimelike(Vertex* a, Vertex* b) {
        int link = getLink(a, b);
        return isTimelike(link);
    }

    /**
     * Check that if the links adjacent to the link spanned by vertex a and b are 
     * of the same nature (timelike or spacelike).
     * @param a First vertex
     * @param b Second vertex
     * @return True if adjacent links have the same type
     */
    bool checkAdjacentSides(Vertex* a, Vertex* b) {
        int link = getLink(a, b);

        return isTimelike((link + 1) % 3) == isTimelike((link + 2) % 3);
    }

    /**
     * Check that if the links adjacent to vertex v are 
     * of the same nature (timelike or spacelike).
     * @param v Vertex
     * @return True if adjacent links have the same type
     */
    bool checkAdjacentSides(Vertex* v) {
        int i = indexFromVertex(v);
        return isTimelike(i) == isTimelike((i + 2) % 3);
    }

    Vertex* getThirdVertex(Vertex* a, Vertex* b) {
        int i = indexFromVertex(a);
        if (vertices[(i + 1) % 3] == b) {
            return vertices[(i + 2) % 3];
        }

        return vertices[(i + 1) % 3];
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
        return i != 1;
    }

    void replaceVertex(Vertex* old, Vertex* n) {
        int i = indexFromVertex(old);
        vertices[i] = n;
    }

    bool isNeighbour(Triangle* t) {
        int count = 0;

        for (int i = 0; i < 3; i++) {
            count += vertices[i]->getTriangles().find(t) !=
                    vertices[i]->getTriangles().end();
        }

        return count == 2;
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
     * Gets the neighbour that has u and v as vertices as well.
     * @param u
     * @param v
     * @return 
     */
    Triangle* getNeighbour(Vertex* u, Vertex* v) {
        Triangle* q, *r;
        Vertex::getAdjacentTriangles(u, v, &q, &r);
        q = q == this ? r : q;
        return q;
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
    
    Vertex* getNextVertex(Vertex* v) {
        int i = indexFromVertex(v);
        return vertices[(i + 1) % 3];
    }
    
    TYPE getType() {
        return type;
    }

private:
    TYPE type;
    Vertex* vertices[3]; /* Each triangle has three vertices */
};

#endif	/* TRIANGLE_H */

