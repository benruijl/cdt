/* 
 * File:   MoveFactory.h
 * Author: bruijl
 *
 * Created on October 9, 2012, 3:43 PM
 */

#ifndef MOVEFACTORY_H
#define	MOVEFACTORY_H

#include "Move.h"
#include "AlexanderMove.h"
#include "FlipMove.h"
#include "InverseAlexanderMove.h"
#include "InverseCollapseMove.h"
#include "CollapseMove.h"
#include "PinchingMove.h"
#include "InversePinchingMove.h"
#include <boost/array.hpp>

class MoveFactory {
private:

    enum MOVES {
        ALEXANDER_TIMELIKE, ALEXANDER_SPACELIKE, FLIP_CHANGE, FLIP,
        COLLAPSE_TIMELIKE, COLLAPSE_SPACELIKE, PINCH_SPACELIKE,
        PINCH_TIMELIKE, COUNT
    };

    boost::array<Move*, COUNT> moves;
    boost::array<Move*, COUNT> invMoves;
    boost::uniform_int<> uint;
public:

    MoveFactory(Simulation& simulation) : uint(0, COUNT - 1) {

        moves[ALEXANDER_SPACELIKE] = new AlexanderMove(false);
        moves[ALEXANDER_TIMELIKE] = new AlexanderMove(true);
        moves[COLLAPSE_SPACELIKE] = new CollapseMove(false);
        moves[COLLAPSE_TIMELIKE] = new CollapseMove(true);
        moves[FLIP] = new FlipMove(true, false);
        moves[FLIP_CHANGE] = new FlipMove(true, true);
        moves[PINCH_SPACELIKE] = new PinchingMove(false);
        moves[PINCH_TIMELIKE] = new PinchingMove(true);

        invMoves[ALEXANDER_SPACELIKE] = new InverseAlexanderMove(false);
        invMoves[ALEXANDER_TIMELIKE] = new InverseAlexanderMove(true);
        invMoves[COLLAPSE_SPACELIKE] = new InverseCollapseMove(false);
        invMoves[COLLAPSE_TIMELIKE] = new InverseCollapseMove(true);
        invMoves[FLIP] = new FlipMove(true, false);
        invMoves[FLIP_CHANGE] = new FlipMove(false, true);
        invMoves[PINCH_SPACELIKE] = new InversePinchingMove(false);
        invMoves[PINCH_TIMELIKE] = new InversePinchingMove(true);

    }

    virtual ~MoveFactory() {
        for (int i = 0; i < COUNT; i++) {
            delete moves[i];
            delete invMoves[i];
        }
    }

    /**
     * Returns a random move. The move should not be freed.
     * @param simulation Simulation
     * @return Random move
     */
    Move* createRandomMove(Simulation& simulation) {
        MOVES move = static_cast<MOVES> (uint(simulation.getRNG()));
        move = COLLAPSE_TIMELIKE; // simulation.getRandomNumber() < 0.5 ? ALEXANDER_SPACELIKE : FLIP;
        bool inverse = simulation.getRandomNumber() < 0.5;

        if (inverse) {
            return invMoves[move]->generateRandomMove(simulation);
        } else {
            return moves[move]->generateRandomMove(simulation);
        }
    }

};

#endif	/* MOVEFACTORY_H */

