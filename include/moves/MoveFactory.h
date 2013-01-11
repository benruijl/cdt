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
public:

    enum MOVES {
        ALEXANDER_TIMELIKE, ALEXANDER_SPACELIKE, FLIP_CHANGE, FLIP,
        COLLAPSE_TIMELIKE, COLLAPSE_SPACELIKE, PINCH_SPACELIKE,
        PINCH_TIMELIKE, COUNT
    };

    MoveFactory(Simulation& simulation);

    virtual ~MoveFactory();

    /**
     * Returns a random move. The move should not be freed.
     * @param simulation Simulation
     * @return Random move
     */
    Move* createRandomMove(Simulation& simulation);

    void setFixedTriangle(Triangle* t);

    /**
     * Adds a move to the filter.
     */
    void addMove(MOVES move);

    /**
     * Adds all moves to the filter
     */
    void addAllMoves();
    
    /**
     * Parses a list of move names to moves and adds them to the system.
     * If the list is empty, all possible moves are added.
     * @param moves List of move names
     */
    void parseMoves(std::vector<std::string> moves);

private:
    boost::array<Move*, COUNT> moves;
    boost::array<Move*, COUNT> invMoves;
    boost::uniform_int<> uint;
    std::vector<MOVES> filter;
};

#endif	/* MOVEFACTORY_H */

