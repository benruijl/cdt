/* 
 * File:   InversePinchingMove.h
 * Author: bruijl
 *
 * Created on October 15, 2012, 1:33 PM
 */

#ifndef INVERSEPINCHINGMOVE_H
#define	INVERSEPINCHINGMOVE_H

#include "Vertex.h"
#include "Triangle.h"

class InversePinchingMove : public Move {
private:
public:

    InversePinchingMove() : Move(4, -2) {
    }

    double getTransitionProbability(VertSet& vertices) {
        return 1.0;
    }

    bool isMovePossible(VertSet& vertices) {
        return false; // TODO
    }

    Move* generateRandomMove(Simulation& simulation) {
        return this;
    }

    double getInverseTransitionProbability(VertSet& vertices) {
        return 1.0; // TODO
    }

    void execute(VertSet& vertices) {
    }
};

#endif	/* INVERSEPINCHINGMOVE_H */

