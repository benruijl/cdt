/* 
 * File:   Triangle.h
 * Author: ben
 *
 * Created on September 19, 2012, 3:31 PM
 */

#ifndef TRIANGLE_H
#define	TRIANGLE_H

#include <iostream>
#include <boost/array.hpp>

class Vertex;

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

    void initialize(TYPE type, Vertex* a, Vertex* b, Vertex* c);

    /**
     * Creates a new triangle. It is automatically registered with the vertices.
     * @param type Type of the triangle
     * @param a First vertex
     * @param b Second vertex
     * @param c Third vertex
     */
    Triangle(TYPE type, Vertex* a, Vertex* b, Vertex* c);

    /**
     * Creates a triangle, given that the links are timelike or spacelike.
     * @param a First vertex
     * @param b Second vertex
     * @param c Third vertex
     * @param timeLikeA First link is timelike
     * @param timeLikeB Second link is timelike
     * @param timeLikeC Third link is timelike
     */
    Triangle(Vertex* a, Vertex* b, Vertex* c, bool timeLikeAB, bool timeLikeBC, bool timeLikeCA);

    int getLink(Vertex* a, Vertex* b);

    int getOppositeLink(Vertex* a);

    bool isOppositeLinkTimelike(Vertex* a);

    /**
     * Checks if a link is timelike.
     * @param link Link number
     * @return True if timelike, false if spacelike
     */
    inline bool isTimelike(int link) {
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
    int getAdjacentTimelikeCount(Vertex* v);

    /**
     * Checks if a link is timelike.
     * @param a First vertex of link 
     * @param b Second vertex of link
     * @return True if timelike, false if spacelike
     */
    bool isTimelike(Vertex* a, Vertex* b);

    /**
     * Check that if the links adjacent to the link spanned by vertex a and b are 
     * of the same nature (timelike or spacelike).
     * @param a First vertex
     * @param b Second vertex
     * @return True if adjacent links have the same type
     */
    bool checkAdjacentSides(Vertex* a, Vertex* b);
    /**
     * Check that if the links adjacent to vertex v are 
     * of the same nature (timelike or spacelike).
     * @param v Vertex
     * @return True if adjacent links have the same type
     */
    bool checkAdjacentSides(Vertex* v);

    Vertex* getThirdVertex(Vertex* a, Vertex* b);

    bool containsVertex(Vertex* a);

    int getLightConeCount(Vertex* v);

    void replaceVertex(Vertex* old, Vertex* n);

    bool isNeighbour(Triangle* t);

    int indexFromVertex(Vertex* v);

    /**
     * Gets the neighbour that has u and v as vertices as well.
     * @param u
     * @param v
     * @return 
     */
    Triangle* getNeighbour(const Vertex* u, const Vertex* v);

    /**
     * Gets the neighbour at a given link.
     * @param link Link
     * @return Neighbour at link 'Link'
     */
    inline Triangle* getNeighbour(int link) {
        return getNeighbour(vertices[link], vertices[(link + 1) % 3]);
    }

    /**
     * Unregisters this triangles at the 3 vertices
     * @return Unregistered triangle ready for deletion
     */
    void removeVertices();

    /**
     * Gets the vertex at a given index.
     */
    inline Vertex* getVertex(int index) {
        return vertices[index];
    }

    /**
     * Gets the next vertex of the triangle. This process is deterministic.
     * @param v Current vertex
     * @return Next vertex
     */
    Vertex* getNextVertex(Vertex* v);

    /**
     * Returns the triangle type.
     */
    TYPE getType() {
        return type;
    }
private:
    TYPE type;
    boost::array<Vertex*, 3 > vertices; /* Each triangle has three vertices */
};

#endif	/* TRIANGLE_H */

