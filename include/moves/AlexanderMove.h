/* 
 * File:   AlexanderMove.h
 * Author: bruijl
 *
 * Created on October 9, 2012, 1:37 PM
 */

#ifndef ALEXANDERMOVE_H
#define	ALEXANDERMOVE_H

#include "Move.h"

class AlexanderMove : public Move {
private:
    Vertex* u, *v;
    bool isTimelike;
public:

    AlexanderMove(bool timelike);

    double getTransitionProbability(std::vector<Vertex*>& vertices);

    bool isMovePossible(std::vector<Vertex*>& vertices);

    Move* generateRandomMove(Simulation& simulation);

    double getInverseTransitionProbability(std::vector<Vertex*>& vertices);

    void execute(std::vector<Vertex*>& vertices);

    std::string printID();
};

#endif	/* ALEXANDERMOVE_H */

