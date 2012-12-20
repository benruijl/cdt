#include "moves/CollapseMove.h"

CollapseMove::CollapseMove(bool timelike) : Move(!timelike * -2, timelike * -2) {
    isTimelike = timelike;
}

double CollapseMove::getTransitionProbability(std::vector<Vertex*>& vertices) {
    return 1.0 / (vertices.size() * u->getNeighbouringVertexCount()) +
            1.0 / (vertices.size() * v->getNeighbouringVertexCount());
}

bool CollapseMove::isMovePossible(std::vector<Vertex*>& vertices) {
    Triangle* first, *second;
    Vertex::getAdjacentTriangles(u, v, &first, &second);

    /* A fixed triangle should not be deleted. */
    if (first == getFixedTriangle() || second == getFixedTriangle()) {
        return false;
    }

    if (first->isTimelike(u, v) != isTimelike || !first->checkAdjacentSides(u, v)
            || !second->checkAdjacentSides(u, v)) {
        return false;
    }

    // topology constraint: because of periodic boundary conditions it could
    // be that the collapse move fails and results in overlapping links
    // note that the vertices should have 2 neighbours in common
    return countOverlap(u->getNeighbouringVertices(), v->getNeighbouringVertices()) == 2;
}

Move* CollapseMove::generateRandomMove(Simulation& simulation) {
    u = simulation.getRandomVertex(simulation.getVertices());
    v = simulation.getRandomElementFromSet(u->getNeighbouringVertices());

    return this;
}

double CollapseMove::getInverseTransitionProbability(std::vector<Vertex*>& vertices) {
    // the inverse move selects two vertices from the surroundings of a
    // chosen central vertex.
    int neighbours = u->getNeighbouringVertexCount() +
            v->getNeighbouringVertexCount() - 4;

    return 1.0 / ((vertices.size() - 1) * neighbours *
            (neighbours - 1) / 2.0);
}

void CollapseMove::execute(std::vector<Vertex*>& vertices) {
    Triangle* first, *second;
    Vertex::getAdjacentTriangles(u, v, &first, &second);

    Vertex* c = first->getThirdVertex(u, v);
    Vertex* d = second->getThirdVertex(u, v);

    first->removeVertices();
    second->removeVertices();

    v->getTriangles() += u->getTriangles();

    foreach(Triangle* t, u->getTriangles()) {
        t->replaceVertex(u, v);
    }

    // remove vertex
    vertices.erase(std::remove(vertices.begin(), vertices.end(), u), vertices.end());

    delete u;
    delete first;
    delete second;

    BOOST_ASSERT(v->checkCausality());
    BOOST_ASSERT(c->checkCausality());
    BOOST_ASSERT(d->checkCausality());
}

std::string CollapseMove::printID() {
    return std::string("COLL_") + (isTimelike ? "TL" : "SL");
}
