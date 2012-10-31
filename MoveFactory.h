/* 
 * File:   MoveFactory.h
 * Author: bruijl
 *
 * Created on October 9, 2012, 3:43 PM
 */

#ifndef MOVEFACTORY_H
#define	MOVEFACTORY_H

#include "Move.h"
#include "Simulation.h"
#include "AlexanderMove.h"
#include "FlipMove.h"
#include "InverseAlexanderMove.h"
#include "InverseCollapseMove.h"
#include "CollapseMove.h"
#include "PinchingMove.h"
#include "InversePinchingMove.h"

class MoveFactory {
private:

    enum MOVES {
        ALEXANDER_TIMELIKE, ALEXANDER_SPACELIKE, FLIP_CHANGE, FLIP,
        COLLAPSE_TIMELIKE, COLLAPSE_SPACELIKE, PINCH, COUNT
    };

    Move* createForwardMove(MOVES move, Simulation& simulation) {
        switch (move) {
            case ALEXANDER_SPACELIKE:
                return (new AlexanderMove(false))->generateRandomMove(simulation);
            case ALEXANDER_TIMELIKE:
                return (new AlexanderMove(true))->generateRandomMove(simulation);
            case COLLAPSE_SPACELIKE:
                return (new CollapseMove(false))->generateRandomMove(simulation);
            case COLLAPSE_TIMELIKE:
                return (new CollapseMove(true))->generateRandomMove(simulation);
            case FLIP:
                return (new FlipMove(true, false))->generateRandomMove(simulation);
            case FLIP_CHANGE:
                return (new FlipMove(true, true))->generateRandomMove(simulation);
            case PINCH:
                return (new PinchingMove())->generateRandomMove(simulation);
            default:
                return NULL;
        }
    }

    Move* createInverseMove(MOVES move, Simulation& simulation) {
        switch (move) {
            case ALEXANDER_SPACELIKE:
                return (new InverseAlexanderMove(false))->generateRandomMove(simulation);
            case ALEXANDER_TIMELIKE:
                return (new InverseAlexanderMove(true))->generateRandomMove(simulation);
            case COLLAPSE_SPACELIKE:
                return (new InverseCollapseMove(false))->generateRandomMove(simulation);
            case COLLAPSE_TIMELIKE:
                return (new InverseCollapseMove(true))->generateRandomMove(simulation);
            case FLIP:
                return (new FlipMove(true, false))->generateRandomMove(simulation);
            case FLIP_CHANGE:
                return (new FlipMove(false, true))->generateRandomMove(simulation);
            case PINCH:
                return (new InversePinchingMove())->generateRandomMove(simulation);
            default:
                return NULL;
        }
    }

public:

    Move* createRandomMove(Simulation& simulation) {
        MOVES move = static_cast<MOVES> ((int) (simulation.getRandomNumber() * COUNT));
        move = COLLAPSE_SPACELIKE; // FIXME: for testing
        bool inverse = simulation.getRandomNumber() < 0.5;

        if (inverse) {
            return createInverseMove(move, simulation);
        } else {
            return createForwardMove(move, simulation);
        }
    }
};

#endif	/* MOVEFACTORY_H */

