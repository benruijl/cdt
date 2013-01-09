#include "moves/FlipMove.h"
#include "Triangle.h"

FlipMove::FlipMove(bool timelike, bool change) :
Move(-2 * change * (timelike * 2 - 1), 2 * change * (timelike * 2 - 1)) {
    isTimelike = timelike;
    this->change = change;
}

double FlipMove::getTransitionProbability(std::vector<Vertex*>& vertices) {
    return 1.0 / (vertices.size() * u->getNeighbouringVertexCount()) +
            1.0 / (vertices.size() * v->getNeighbouringVertexCount());
}

bool FlipMove::isMovePossible(std::vector<Vertex*>& vertices) {
    Triangle* first, *second;
    Vertex::getAdjacentTriangles(u, v, &first, &second);
    Vertex* c = first->getThirdVertex(u, v);
    Vertex* d = second->getThirdVertex(u, v);

    /* A fixed triangle should not be deleted. */
    if (first == getFixedTriangle() || second == getFixedTriangle()) {
        return false;
    }

    // prevent link overlap, could happen in a very rare cases
    VertSet verts = c->getNeighbouringVertices();
    if (verts.find(d) != verts.end()) {
        return false;
    }

    return first->isTimelike(u, v) == isTimelike && first->isTimelike(u, c) != second->isTimelike(u, d)
            && first->isTimelike(v, c) != second->isTimelike(v, d);
}

Move* FlipMove::generateRandomMove(Simulation& simulation) {
    u = simulation.getRandomVertex(simulation.getVertices());
    v = simulation.getRandomElementFromSet(u->getNeighbouringVertices());
    return this;
}

double FlipMove::getInverseTransitionProbability(std::vector<Vertex*>& vertices) {
    Triangle* first, *second;
    Vertex::getAdjacentTriangles(u, v, &first, &second);

    Vertex* c = first->getThirdVertex(u, v);
    Vertex* d = second->getThirdVertex(u, v);

    return 1.0 / (vertices.size() * (c->getNeighbouringVertexCount() + 1)) +
            1.0 / (vertices.size() * (d->getNeighbouringVertexCount() + 1));
}

void FlipMove::execute(std::vector<Vertex*>& vertices) {
    Triangle* first, *second;
    Vertex::getAdjacentTriangles(u, v, &first, &second);

    Vertex* c = first->getThirdVertex(u, v);
    Vertex* d = second->getThirdVertex(u, v);

    /* Get link types */
    bool lNew = first->isTimelike(u, v) ^ change;
    bool lAC = first->isTimelike(u, c);
    bool lCB = first->isTimelike(c, v);
    bool lAD = second->isTimelike(u, d);
    bool lBD = second->isTimelike(v, d);

    new Triangle(u, c, d, lAC, lNew, lAD);
    new Triangle(c, v, d, lCB, lBD, lNew);

    first->removeVertices();
    second->removeVertices();
    delete first;
    delete second;

    BOOST_ASSERT(u->checkCausality());
    BOOST_ASSERT(v->checkCausality());
    BOOST_ASSERT(c->checkCausality());
    BOOST_ASSERT(d->checkCausality());
}

std::string FlipMove::printID() {
    return std::string("FLIP_") + (isTimelike ? "TL" : "SL") + (change ? "_CH" : "");
}
