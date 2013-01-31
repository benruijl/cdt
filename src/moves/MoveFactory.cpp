#include "moves/MoveFactory.h"
#include "Config.h" 
#include <boost/assign.hpp>
#include <boost/unordered/unordered_map.hpp>
#include <boost/tokenizer.hpp>

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
    
    loadMoves();
}

MoveFactory::~MoveFactory() {
    for (int i = 0; i < COUNT; i++) {
        delete moves[i];
        delete invMoves[i];
    }
}

void MoveFactory::addMove(MoveFactory::MOVES move) {
    filter.push_back(move);
    uint = boost::uniform_int<>(0, filter.size() - 1);

}

/**
 * Adds all moves to the filter
 */
void MoveFactory::addAllMoves() {
    for (int i = 0; i < COUNT; i++) {
        filter.push_back(static_cast<MOVES> (i));
    }
    uint = boost::uniform_int<>(0, filter.size() - 1);
}

Move* MoveFactory::createRandomMove(Simulation& simulation) {
    BOOST_ASSERT(filter.size() > 0);

    MOVES move = filter[uint(simulation.getRNG())];
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

void MoveFactory::parseMoves(std::vector<std::string> moves) {
    boost::unordered_map<std::string, MOVES> m =
            boost::assign::map_list_of("alexander_tl", ALEXANDER_TIMELIKE)
            ("alexander_sl", ALEXANDER_SPACELIKE)
            ("collapse_tl", COLLAPSE_TIMELIKE)
            ("collapse_sl", COLLAPSE_SPACELIKE)
            ("flip", FLIP)
            ("flip_change", FLIP_CHANGE)
            ("pinching_tl", PINCH_TIMELIKE)
            ("pinching_sl", PINCH_SPACELIKE);

    if (moves.size() == 0) {
        addAllMoves();
        return;
    }

    foreach(std::string& move, moves) {
        boost::unordered_map<std::string, MOVES>::iterator it =
                m.find(move);

        if (it != m.end()) {
            addMove(it->second);
        } else {
            std::cerr << "Unrecognized move: " << move << std::endl;
        }
    }
}

void MoveFactory::loadMoves() {
    std::string moves = Config::getInstance().getPropertyTree().
            get("general.moves", std::string("")); // empty means all moves
    boost::char_separator<char> sep(" ,");
    boost::tokenizer< boost::char_separator<char> > tok(moves, sep);

    std::vector<std::string> m;
    foreach(const std::string& move, tok) {
        m.push_back(move);
    }
    
    parseMoves(m);
}

