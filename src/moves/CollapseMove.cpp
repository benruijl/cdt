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
    return countOverlap(neighboursU, v->getNeighbouringVertices()) == 2;
}

Move* CollapseMove::generateRandomMove(Simulation& simulation) {
    uIndex = simulation.getRandomInt(0, simulation.getVertices().size() - 1);
    u = simulation.getVertices()[uIndex];
    neighboursU = u->getNeighbouringVertices();
    v = simulation.getRandomElementFromSet(neighboursU);

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

    first->removeFromVertices();
    second->removeFromVertices();

    TriSet left = u->getTriangles();
    v->getTriangles() += left;

    foreach(Triangle* t, left) {
        t->replaceVertex(u, v);
    }

    // remove vertex
    vertices[uIndex] = vertices.back();
    vertices.resize(vertices.size() - 1);

    delete u;
    delete first;
    delete second;

    Vertex::getAdjacentTriangles(v, c, &first, &second);
    Vertex* tl = *v->getSectorVertices(first, true, true).begin();

    if (isTimelike && hasSelfOverlappingBubbles(tl, v)) {
        //std::cout << "Self-overlapping bubble created, reverting collapse" << std::endl;
        // Collected all triangles on one side of the barrier v - u - w

        // revert
        Vertex* x = new Vertex();
        v->getTriangles() -= left;
        x->getTriangles() += left;

        foreach(Triangle* t, left) {
            t->replaceVertex(v, x);
        }

        // Create two new triangles
        new Triangle(c, x, v, false, true, false);
        new Triangle(d, x, v, false, true, false);

        vertices.push_back(x);
    }

    //std::cout << "C " << isTimelike << " " << c << " " << v << " " << d << std::endl;

    BOOST_ASSERT(v->checkCausality());
    BOOST_ASSERT(c->checkCausality());
    BOOST_ASSERT(d->checkCausality());
}

std::string CollapseMove::printID() {
    return std::string("COLL_") + (isTimelike ? "TL" : "SL");
}
