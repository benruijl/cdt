/* 
 * File:   CollapseMove.h
 * Author: bruijl
 *
 * Created on October 9, 2012, 2:24 PM
 */

#ifndef COLLAPSEMOVE_H
#define	COLLAPSEMOVE_H

#include "Move.h"

class CollapseMove : public Move {
private:
    Vertex* u, *v;
    bool isTimelike;

public:

    CollapseMove(bool timelike);

    double getTransitionProbability(std::vector<Vertex*>& vertices);

    bool isMovePossible(std::vector<Vertex*>& vertices);

    Move* generateRandomMove(Simulation& simulation);

    double getInverseTransitionProbability(std::vector<Vertex*>& vertices);

    void execute(std::vector<Vertex*>& vertices);

    std::string printID();
};

#endif	/* COLLAPSEMOVE_H */

