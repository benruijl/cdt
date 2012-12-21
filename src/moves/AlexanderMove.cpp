#include "moves/AlexanderMove.h"
#include "Triangle.h"

AlexanderMove::AlexanderMove(bool timelike) : Move(!timelike * 2, timelike * 2) {
    isTimelike = timelike;
}

double AlexanderMove::getTransitionProbability(std::vector<Vertex*>& vertices) {
    return 1.0 / (vertices.size() * u->getNeighbouringVertexCount()) +
            1.0 / (vertices.size() * v->getNeighbouringVertexCount());
}

bool AlexanderMove::isMovePossible(std::vector<Vertex*>& vertices) {
    Triangle* first, *second;
    Vertex::getAdjacentTriangles(u, v, &first, &second);

#ifdef ALEXANDER_COMPARE_COLLAPSE
    if (!first->checkAdjacentSides(u, v) ||
            !second->checkAdjacentSides(u, v)) {
        return false;
    }
#endif

    /* A fixed triangle should not be deleted. */
    if (first == getFixedTriangle() || second == getFixedTriangle()) {
        return false;
    }

    return isTimelike == first->isTimelike(u, v);
}

Move* AlexanderMove::generateRandomMove(Simulation& simulation) {
    u = simulation.getRandomVertex(simulation.getVertices());
    v = simulation.getRandomElementFromSet(u->getNeighbouringVertices());
    return this;
}

double AlexanderMove::getInverseTransitionProbability(std::vector<Vertex*>& vertices) {
    // Alexander moves create one new vertex and the inverse needs
    // only this vertex to perform the move
    return 1.0 / (vertices.size() + 1);
}

void AlexanderMove::execute(std::vector<Vertex*>& vertices) {
    Triangle* first, *second;
    Vertex::getAdjacentTriangles(u, v, &first, &second);

    Vertex* c = first->getThirdVertex(u, v);
    Vertex* d = second->getThirdVertex(u, v);

    /* Get link types */
    bool lAB = first->isTimelike(u, v);
    bool lAC = first->isTimelike(u, c);
    bool lCB = first->isTimelike(c, v);
    bool lAD = second->isTimelike(u, d);
    bool lBD = second->isTimelike(v, d);
    bool newLink = !lAB;

    Vertex* e = new Vertex(); // new vertex
    new Triangle(u, c, e, lAC, newLink, lAB);
    new Triangle(e, c, v, newLink, lCB, lAB);
    new Triangle(u, e, d, lAB, newLink, lAD);
    new Triangle(e, v, d, lAB, lBD, newLink);

    vertices.push_back(e);
    first->removeVertices();
    second->removeVertices();
    delete first;
    delete second;

    BOOST_ASSERT(u->checkCausality());
    BOOST_ASSERT(v->checkCausality());
    BOOST_ASSERT(c->checkCausality());
    BOOST_ASSERT(d->checkCausality());
    BOOST_ASSERT(e->checkCausality());
}

std::string AlexanderMove::printID() {
    return std::string("ALEX_") + (isTimelike ? "TL" : "SL");
}
