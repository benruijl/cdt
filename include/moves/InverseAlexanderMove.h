/* 
 * File:   InverseAlexanerMove.h
 * Author: bruijl
 *
 * Created on October 9, 2012, 3:08 PM
 */

#ifndef INVERSEALEXANDERMOVE_H
#define	INVERSEALEXANDERMOVE_H

#include "Move.h"

class InverseAlexanderMove : public Move {
private:
    Vertex* u;
    bool isTimelike;

    /**
     * Check if the moves does not cause two links to overlap.
     * @return True if move is valid
     */
    bool doTopologyCheck();
public:

    InverseAlexanderMove(bool timelike);

    double getTransitionProbability(std::vector<Vertex*>& vertices);

    bool isMovePossible(std::vector<Vertex*>& vertices);

    Move* generateRandomMove(Simulation& simulation);

    double getInverseTransitionProbability(std::vector<Vertex*>& vertices);

    void execute(std::vector<Vertex*>& vertices);

    std::string printID();
};

#endif	/* INVERSEALEXANERMOVE_H */

