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
    Triangle* t1 = new Triangle(u, c, e, lAC, newLink, lAB);
    Triangle* t2 = new Triangle(e, c, v, newLink, lCB, lAB);
    Triangle* t3 = new Triangle(u, e, d, lAB, newLink, lAD);
    Triangle* t4 = new Triangle(e, v, d, lAB, lBD, newLink);

    vertices.push_back(e);
    first->removeFromVertices();
    second->removeFromVertices();
    delete first;
    delete second;

    // FIXME: narrow bubble check down to particular alexander moves
    if ((lAC && hasSelfOverlappingBubbles(u, c)) ||
            (lAD && hasSelfOverlappingBubbles(u, d))) {
        //std::cout << "Self-overlapping bubble created, reverting alex" << std::endl;

        t1->removeFromVertices();
        t2->removeFromVertices();
        t3->removeFromVertices();
        t4->removeFromVertices();

        delete e;
        delete t1;
        delete t2;
        delete t3;
        delete t4;
        vertices.resize(vertices.size() - 1); // remove e

        // revert move
        new Triangle(u, c, v, lAC, lCB, isTimelike);
        new Triangle(u, d, v, lAD, lBD, isTimelike);
        return;
    }

    BOOST_ASSERT(u->checkCausality());
    BOOST_ASSERT(v->checkCausality());
    BOOST_ASSERT(c->checkCausality());
    BOOST_ASSERT(d->checkCausality());
    BOOST_ASSERT(e->checkCausality());

    //std::cout << printID() << " " << lAC << lCB << lBD << lAD << " " << u << " " << v << std::endl;
}

std::string AlexanderMove::printID() {
    return std::string("ALEX_") + (isTimelike ? "TL" : "SL");
}
