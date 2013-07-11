#include "moves/InverseCollapseMove.h"

TriSet InverseCollapseMove::getBarrierTriangles(bool left) {
    Triangle* first, *second;
    Vertex::getAdjacentTriangles(u, v, &first, &second);

    // Collected all triangles on one side of the barrier v - u - w
    TriSet tri;
    Triangle* cur = left ? second : first;
    Vertex* edgeVertex = v;

    while (edgeVertex != w) {
        cur = cur->getNeighbour(u, edgeVertex);
        edgeVertex = cur->getThirdVertex(u, edgeVertex);
        tri.insert(cur);
    }

    return tri;
}

InverseCollapseMove::InverseCollapseMove(bool timelike) : Move(!timelike * 2, timelike * 2) {
    isTimelike = timelike;
}

double InverseCollapseMove::getTransitionProbability(std::vector<Vertex*>& vertices) {
    return 1.0 / (vertices.size() * u->getNeighbouringVertexCount() *
            (u->getNeighbouringVertexCount() - 1) / 2.0);
}

bool InverseCollapseMove::isMovePossible(std::vector<Vertex*>& vertices) {
    Triangle* first, *second;
    Vertex::getAdjacentTriangles(u, v, &first, &second);
    bool lUV = first->isTimelike(u, v);

    // if the number of sector transitions is two, the link type of u an w 
    // is the same and they are in a different sector
    return lUV != isTimelike && u->isInOtherSector(first, v, w);
}

Move* InverseCollapseMove::generateRandomMove(Simulation& simulation) {
    u = simulation.getRandomVertex(simulation.getVertices());
    VertSet neighbours = u->getNeighbouringVertices();
    v = simulation.getRandomElementFromSet(neighbours);
    neighbours.erase(v);
    w = simulation.getRandomElementFromSet(neighbours);

    return this;
}

double InverseCollapseMove::getInverseTransitionProbability(std::vector<Vertex*>& vertices) {
    int numTriLeft = getBarrierTriangles(true).size();

    // the probability of the inverse is selecting a vertex and then one of their neighbours.
    // The number of neighbours depends on the vertex and is calculated in numTriLeft.
    return 1.0 / ((vertices.size() + 1) * (numTriLeft + 2)) +
            1.0 / ((vertices.size() + 1) *
            (u->getTriangles().size() - numTriLeft + 2));
}

void InverseCollapseMove::execute(std::vector<Vertex*>& vertices) {
    Triangle* first, *second;
    Vertex::getAdjacentTriangles(u, v, &first, &second);
    bool lUV = first->isTimelike(u, v);

    // Collected all triangles on one side of the barrier v - u - w
    TriSet tri = getBarrierTriangles(true);

    // Create new vertex
    Vertex* x = new Vertex();
    u->getTriangles() -= tri;
    x->getTriangles() += tri;

    foreach(Triangle* t, tri) {
        t->replaceVertex(u, x);
    }

    // Create two new triangles
    new Triangle(v, x, u, lUV, !lUV, lUV);
    new Triangle(w, x, u, lUV, !lUV, lUV);

    vertices.push_back(x);

    BOOST_ASSERT(u->checkCausality());
    BOOST_ASSERT(v->checkCausality());
    BOOST_ASSERT(w->checkCausality());
    BOOST_ASSERT(x->checkCausality());
    
    if (isTimelike && hasSelfOverlappingBubbles(x, u)) {
        std::cout << "SELF OVERLAPPING BUBBLES!" << std::endl;
    }
}

std::string InverseCollapseMove::printID() {
    return std::string("INV_COLL_") + (isTimelike ? "TL" : "SL");
}
