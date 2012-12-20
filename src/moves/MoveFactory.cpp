#include "moves/MoveFactory.h" 

MoveFactory::MoveFactory(Simulation& simulation) : uint(0, COUNT - 1) {

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

MoveFactory::~MoveFactory() {
    for (int i = 0; i < COUNT; i++) {
        delete moves[i];
        delete invMoves[i];
    }
}

Move* MoveFactory::createRandomMove(Simulation& simulation) {
    MOVES move = static_cast<MOVES> (uint(simulation.getRNG()));
    move = simulation.getRandomNumber() < 0.5 ? COLLAPSE_SPACELIKE :
            FLIP_CHANGE;
    bool inverse = simulation.getRandomNumber() < 0.5;

    if (inverse) {
        return invMoves[move]->generateRandomMove(simulation);
    } else {
        return moves[move]->generateRandomMove(simulation);
    }
}

void MoveFactory::setFixedTriangle(Triangle* t) {
    for (int i = 0; i < COUNT; i++) {
        moves[i]->setFixedTriangle(t);
        invMoves[i]->setFixedTriangle(t);
    }
}

