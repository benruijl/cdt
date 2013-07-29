#include "moves/InverseAlexanderMove.h"    

bool InverseAlexanderMove::doTopologyCheck() {
    // TODO: improve code
    Triangle *first, *second, *third, *fourth, *t, *r;
    Vertex* v = u->getNeighbouringVertex();
    Vertex::getAdjacentTriangles(u, v, &first, &second);
    Vertex* w = first->getThirdVertex(u, v);
    Vertex::getAdjacentTriangles(u, w, &t, &r);
    third = t == first ? r : t;
    Vertex* x = third->getThirdVertex(u, w);
    Vertex::getAdjacentTriangles(u, x, &t, &r);
    fourth = t == third ? r : t;
    Vertex* y = fourth->getThirdVertex(u, x);

    if (first->isTimelike(u, v) != isTimelike) {
        v = w;
        x = y;
    }

    return !v->isNeighbour(x);
}

InverseAlexanderMove::InverseAlexanderMove(bool timelike) : Move(!timelike * -2, timelike * -2) {
    isTimelike = timelike;
}

double InverseAlexanderMove::getTransitionProbability(std::vector<Vertex*>& vertices) {
    return 1.0 / vertices.size();
}

bool InverseAlexanderMove::isMovePossible(std::vector<Vertex*>& vertices) {
    // vertex needs to have 4 links: two spacelike and two timelike
    if (u->getNeighbouringVertexCount() != 4) {
        return false;
    }

    if (!doTopologyCheck()) {
        return false;
    }

    int tlCount = 0;

    foreach(Triangle* t, u->getTriangles()) {
        tlCount += t->isOppositeLinkTimelike(u);
    }


    // The check for tlCount == 2 is more complicated
    // TODO: find a better way
    Triangle *first, *second, *third, *fourth, *t, *r;
    Vertex* v = u->getNeighbouringVertex();
    Vertex::getAdjacentTriangles(u, v, &first, &second);
    Vertex* w = first->getThirdVertex(u, v);
    Vertex::getAdjacentTriangles(u, w, &t, &r);
    third = t == first ? r : t;
    Vertex* x = third->getThirdVertex(u, w);
    Vertex::getAdjacentTriangles(u, x, &t, &r);
    fourth = t == third ? r : t;
    Vertex* y = fourth->getThirdVertex(u, x);

    /* A fixed triangle should not be deleted. */
    if (first == getFixedTriangle() || second == getFixedTriangle() ||
            third == getFixedTriangle() || fourth == getFixedTriangle()) {
        return false;
    }

#ifdef ALEXANDER_COMPARE_COLLAPSE
    return tlCount == 4;
#endif

    if (tlCount != 2) {
        return (isTimelike && tlCount <= 2) || (!isTimelike && tlCount >= 2);

    }

    bool lUV = first->isTimelike(u, v);
    bool lVW = first->isTimelike(v, w);
    bool lWX = third->isTimelike(w, x);
    bool lXU = third->isTimelike(x, u);
    bool lXY = fourth->isTimelike(x, y);
    bool lYV = second->isTimelike(y, v);
    bool lYU = second->isTimelike(y, u);
    bool lUW = first->isTimelike(u, w);

    // test the symmetric configuration
    if (first->isOppositeLinkTimelike(u) != second->isOppositeLinkTimelike(u) &&
            first->isOppositeLinkTimelike(u) != third->isOppositeLinkTimelike(u)) {
#ifdef ALEXANDER_DISABLE_BUBBLE_COLLAPSE
        if (isTimelike) {
            return false;
        }
#endif

        return true;
    }

    if (lUV == lVW && lVW == lWX && lWX == lXU) return first->isTimelike(u, v) != isTimelike;
    if (lUW == lWX && lWX == lXY && lXY == lYU) return first->isTimelike(u, w) != isTimelike;
    if (lXY == lYV && lYV == lUV && lUV == lXU) return first->isTimelike(u, v) != isTimelike;
    if (lUW == lVW && lVW == lYV && lYV == lYU) return first->isTimelike(u, w) != isTimelike;

    BOOST_ASSERT(false);
}

Move* InverseAlexanderMove::generateRandomMove(Simulation& simulation) {
    uIndex = simulation.getRandomInt(0, simulation.getVertices().size() - 1);
    u = simulation.getVertices()[uIndex];
    neighboursU = u->getNeighbouringVertices();
    return this;
}

double InverseAlexanderMove::getInverseTransitionProbability(std::vector<Vertex*>& vertices) {
    Triangle *first, *second;
    double prob = 0;

    foreach(Vertex* v, neighboursU) {
        Vertex::getAdjacentTriangles(u, v, &first, &second);

        if (first->isTimelike(u, v) == isTimelike) {
            // -1 because vertex u does not exist
            prob += 1.0 / ((vertices.size() - 1) *
                    (v->getNeighbouringVertexCount()));
        }
    }

    return prob;
}

void InverseAlexanderMove::execute(std::vector<Vertex*>& vertices) {
    Triangle *first, *second, *third, *fourth, *t, *r;
    Vertex *v, *w, *x, *y;
    bool lUV, lVW, lVY, lWX, lXY;

    /* Determine the surroundings in a deterministic way */
    v = *neighboursU.begin();
    Vertex::getAdjacentTriangles(u, v, &first, &second);
    w = first->getThirdVertex(u, v);
    y = second->getThirdVertex(u, v);

    lUV = first->isTimelike(u, v);
    lVW = first->isTimelike(v, w);
    lVY = second->isTimelike(v, y);

    Vertex::getAdjacentTriangles(u, w, &t, &r);
    third = t == first ? r : t;

    x = third->getThirdVertex(u, w);
    Vertex::getAdjacentTriangles(u, x, &t, &r);
    fourth = t == third ? r : t;

    lWX = third->isTimelike(w, x);
    lXY = fourth->isTimelike(x, y);
    
    /* Perform cleanup */
    vertices[uIndex] = vertices.back();
    vertices.resize(vertices.size() - 1);

    first->removeFromVertices();
    second->removeFromVertices();
    third->removeFromVertices();
    fourth->removeFromVertices();
    delete first;
    delete second;
    delete third;
    delete fourth;
    delete u;

    if (lUV != isTimelike) {
        new Triangle(v, w, y, lVW, !lUV, lVY);
        new Triangle(w, x, y, lWX, lXY, !lUV);
    } else {
        new Triangle(v, w, x, lVW, lWX, lUV);
        new Triangle(v, x, y, lUV, lXY, lVY);
    }

    //  check for self-overlapping bubbles
    // FIXME: last check == 2 too imprecise, should be 1100, 0110, etc.
   // if (lVW + lWX + lXY + lVY == 1 || (isTimelike && lVW + lWX + lXY + lVY == 2)) {
    if (isTimelike) {
        if ((lUV && hasSelfOverlappingBubbles(v, x)) ||
                (!lUV && hasSelfOverlappingBubbles(w, y))) {
            //std::cout << "Self-overlapping bubble created, reverting inv alex" << std::endl;

            if (lUV) {
                Vertex::getAdjacentTriangles(v, x, &first, &second);
            } else {
                Vertex::getAdjacentTriangles(w, y, &first, &second);
            }

            // revert
            u = new Vertex();
            new Triangle(u, v, w, lUV, lVW, !lUV);
            new Triangle(u, w, x, !lUV, lWX, lUV);
            new Triangle(u, x, y, lUV, lXY, !lUV);
            new Triangle(u, y, v, !lUV, lVY, lUV);

            vertices.push_back(u);
            first->removeFromVertices();
            second->removeFromVertices();
            delete first;
            delete second;
        }
    }


    BOOST_ASSERT(v->checkCausality());
    BOOST_ASSERT(w->checkCausality());
    BOOST_ASSERT(x->checkCausality());
    BOOST_ASSERT(y->checkCausality());

    //std::cout << printID() << " " << lVW << lWX << lXY << lVY <<  " " << u << std::endl;
}

std::string InverseAlexanderMove::printID() {
    return std::string("INV_ALEX_") + (isTimelike ? "TL" : "SL");
}
