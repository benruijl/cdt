/* 
 * File:   InversePinchingMove.h
 * Author: bruijl
 *
 * Created on October 15, 2012, 1:33 PM
 */

#ifndef INVERSEPINCHINGMOVE_H
#define	INVERSEPINCHINGMOVE_H

#include "Move.h"

class InversePinchingMove : public Move {
private:
    Vertex* v;
    Triangle *t, *s;
    bool left; // orientation of the asymmetric expansion
    bool isTimelike;
public:

    InversePinchingMove(bool isTimelike);

    double getTransitionProbability(std::vector<Vertex*>& vertices);

    bool isMovePossible(std::vector<Vertex*>& vertices);

    Move* generateRandomMove(Simulation& simulation);

    double getInverseTransitionProbability(std::vector<Vertex*>& vertices);

    void execute(std::vector<Vertex*>& vertices);
    std::string printID();
};

#endif	/* INVERSEPINCHINGMOVE_H */

