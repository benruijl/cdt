/* 
 * File:   Moves.h
 * Author: bruijl
 *
 * Created on October 4, 2012, 11:40 AM
 */

#ifndef MOVES_H
#define	MOVES_H

#include "Triangle.h"

class Moves {
public:

    enum MOVES {
        ALEXANDER_TIMELIKE, ALEXANDER_SPACELIKE, FLIP_CHANGE, FLIP,
        COLLAPSE_TIMELIKE, COLLAPSE_SPACELIKE, COUNT
    };

    Moves(VertSet& vertices);
    Moves(const Moves& orig);
    virtual ~Moves();
    
    /**
     * Return a random move.
     * @param r Random number
     * @return 
     */
    MOVES generateRandomMove(double r) {
        return static_cast<MOVES>((int)(r * COUNT));
    }

    /**
     * Gets the Boltzmann probability of a move, i.e. P(x')/P(x)
     * @param move The move type
     * @param inv Is the move inverted?
     * @param lambda Action parameter
     * @param alpha Ratio between timelike and spacelike lengths
     * @return 
     */
    double getMoveProbability(MOVES move, bool inv, double lambda, double alpha);

    /**
     * Performs a move on the link spanned by vertex a and b.
     */
    Vertex* doMove(Vertex* a, Vertex* b, MOVES move);
    
    int getInverseMoveCount(MOVES move, Vertex* a, Vertex* b);

    /**
     * Finds the number of ways a collapse move can be inverted.
     * @param t First vertex of link that should be collapsed
     * @param u First vertex of link that should be collapsed
     * @return 
     */
    int getInverseCollapseMoveCount(Vertex* u, Vertex* v);
private:
    VertSet& vertices;
    
    /**
     * Do the collapse move. It removes all the occurrences of a and replaces it with b.
     * This move is not always possible.
     * TODO: move to separate Moves class
     */
    Vertex* doCollapse(Vertex* a, Vertex* b);

    /**
     * A move that flips a link. 
     * @param a First link vertex
     * @param b Second link vertex
     * @param change Should the link type change?
     * @return 
     */
    Vertex* doFlip(Vertex* a, Vertex* b, bool change);

    /**
     * An Alexander move is making a cross in a diamons shape. An Alexandar move
     * is always possible.
     * @param a First link vertex
     * @param b Second link vertex
     * @return 
     */
    Vertex* doAlexander(Vertex* a, Vertex* b);

    /**
     * Do the inverse of an Alexander move. It needs four links.
     * @param a
     * @param b
     * @param c
     * @param u
     * @return 
     */
    Vertex* doInverseAlexander(Vertex* a, Vertex* b, Vertex* u);

    /**
     * Inverts a collapse. Links a-b and b-c should be of the same type.
     * @param a First vertex
     * @param b Second vertex
     * @param c Third vertex
     * @return Vertex b
     */
    Vertex* doInverseCollapse(Vertex* a, Vertex* b, Vertex* c);

};

#endif	/* MOVES_H */

