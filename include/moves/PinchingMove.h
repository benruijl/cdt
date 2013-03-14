/* 
 * File:   PinchingMove.h
 * Author: bruijl
 *
 * Created on October 15, 2012, 10:45 AM
 */

#ifndef PINCHINGMOVE_H
#define	PINCHINGMOVE_H

#include "Move.h"

class PinchingMove : public Move {
private:
    Vertex *u, *v;
    VertSet neighboursU;
    bool isTimelike;
public:

    PinchingMove(bool isTimelike);

    double getTransitionProbability(std::vector<Vertex*>& vertices);

    bool isMovePossible(std::vector<Vertex*>& vertices);

    Move* generateRandomMove(Simulation& simulation);

    double getInverseTransitionProbability(std::vector<Vertex*>& vertices);

    void execute(std::vector<Vertex*>& vertices);

    std::string printID();
};

#endif	/* PINCHINGMOVE_H */

