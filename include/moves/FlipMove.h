/* 
 * File:   FlipMove.h
 * Author: bruijl
 *
 * Created on October 9, 2012, 2:34 PM
 */

#ifndef FLIPMOVE_H
#define	FLIPMOVE_H

#include "Move.h"

class FlipMove : public Move {
private:
    Vertex* u, *v;
    bool isTimelike, change;
public:

    /**
     * A flip move.
     * The inverse flip move is a move with spacelike instead of timelike.
     * @param timelike Is the link timelike?
     * @param change Should the timelike link change to spacelike and vice versa?
     */
    FlipMove(bool timelike, bool change);

    double getTransitionProbability(std::vector<Vertex*>& vertices);

    bool isMovePossible(std::vector<Vertex*>& vertices);

    Move* generateRandomMove(Simulation& simulation);
    double getInverseTransitionProbability(std::vector<Vertex*>& vertices);

    void execute(std::vector<Vertex*>& vertices);

    std::string printID();
};

#endif	/* FLIPMOVE_H */

