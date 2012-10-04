/* 
 * File:   Moves.cpp
 * Author: bruijl
 * 
 * Created on October 4, 2012, 11:40 AM
 */

#include "Moves.h"

Moves::Moves() {
}

Moves::Moves(const Moves& orig) {
}

Moves::~Moves() {
}

double Moves::getMoveProbability(MOVES move, bool inv, double lambda, double alpha) {
    int Ntts = 0, Ntss = 0;
    int inv_mod = inv ? 1 : -1;

    switch (move) {
        case COLLAPSE_SPACELIKE:
        case ALEXANDER_SPACELIKE:
            Ntss = 0;
            Ntts = 2;
            break;
        case COLLAPSE_TIMELIKE:
        case ALEXANDER_TIMELIKE:
            Ntss = 2;
            Ntts = 0;
            break;
        case FLIP_CHANGE:
            Ntss = 2;
            Ntts = -2;
            break;
    }

    return exp(inv_mod * lambda / 4.0 * (sqrt(5) * Ntts + sqrt(3) * Ntss));
}

Vertex* Moves::doCollapse(Vertex* a, Vertex* b) {
    Triangle* first, *second;
    Vertex::getAdjacentTriangles(a, b, &first, &second);

    Vertex* c = first->getThirdVertex(a, b);
    Vertex* d = second->getThirdVertex(a, b);

    /* A collapse is only valid if the to be merged links are of the same type */
    BOOST_ASSERT(first->checkAdjacentSides(a, b));
    BOOST_ASSERT(second->checkAdjacentSides(a, b));

    first->removeVertices();
    second->removeVertices();

    b->getTriangles() += a->getTriangles();

    foreach(Triangle* t, a->getTriangles()) {
        t->replaceVertex(a, b);
    }

    delete a;
    delete first;
    delete second;

    BOOST_ASSERT(b->checkCausality());
    BOOST_ASSERT(c->checkCausality());
    BOOST_ASSERT(d->checkCausality());

    return b;
}

Vertex* Moves::doFlipChange(Vertex* a, Vertex* b) {
    Triangle* first, *second;
    Vertex::getAdjacentTriangles(a, b, &first, &second);

    Vertex* c = first->getThirdVertex(a, b);
    Vertex* d = second->getThirdVertex(a, b);

    /* Get link types */
    bool lAB = first->isTimelike(a, b);
    bool lAC = first->isTimelike(a, c);
    bool lCB = first->isTimelike(c, b);
    bool lAD = second->isTimelike(a, d);
    bool lBD = second->isTimelike(b, d);

    new Triangle(a, c, d, lAC, !lAB, lAD);
    new Triangle(c, b, d, lCB, lBD, !lAB);

    first->removeVertices();
    second->removeVertices();
    delete first;
    delete second;

    BOOST_ASSERT(a->checkCausality());
    BOOST_ASSERT(b->checkCausality());
    BOOST_ASSERT(c->checkCausality());
    BOOST_ASSERT(d->checkCausality());

    return c;
}

Vertex* Moves::doFlip(Vertex* a, Vertex* b) {
    Triangle* first, *second;
    Vertex::getAdjacentTriangles(a, b, &first, &second);

    Vertex* c = first->getThirdVertex(a, b);
    Vertex* d = second->getThirdVertex(a, b);

    a->getTriangles().erase(second);
    c->getTriangles().insert(second);
    second->replaceVertex(a, c);

    b->getTriangles().erase(first);
    d->getTriangles().insert(first);
    first->replaceVertex(b, d);

    BOOST_ASSERT(a->checkCausality());
    BOOST_ASSERT(b->checkCausality());
    BOOST_ASSERT(c->checkCausality());
    BOOST_ASSERT(d->checkCausality());

    return c;
}

Vertex* Moves::doAlexander(Vertex* a, Vertex* b) {
    Triangle* first, *second;
    Vertex::getAdjacentTriangles(a, b, &first, &second);

    Vertex* c = first->getThirdVertex(a, b);
    Vertex* d = second->getThirdVertex(a, b);

    /* Get link types */
    bool lAB = first->isTimelike(a, b);
    bool lAC = first->isTimelike(a, c);
    bool lCB = first->isTimelike(c, b);
    bool lAD = second->isTimelike(a, d);
    bool lBD = second->isTimelike(b, d);
    bool newLink = !lAB;

    Vertex* u = new Vertex(); // new vertex
    new Triangle(a, c, u, lAC, newLink, lAB);
    new Triangle(u, c, b, newLink, lCB, lAB);
    new Triangle(a, u, d, lAB, newLink, lAD);
    new Triangle(u, b, d, lAB, lBD, newLink);

    first->removeVertices();
    second->removeVertices();
    delete first;
    delete second;

    BOOST_ASSERT(a->checkCausality());
    BOOST_ASSERT(b->checkCausality());
    BOOST_ASSERT(c->checkCausality());
    BOOST_ASSERT(d->checkCausality());
    BOOST_ASSERT(u->checkCausality());

    return u;
}

Vertex* Moves::doInverseAlexander(Vertex* a, Vertex* b, Vertex* u) {
    Triangle* first, *second, *third, *fourth;
    Vertex::getAdjacentTriangles(a, u, &first, &second);

    Vertex* c = first->getThirdVertex(a, u);
    Vertex* d = second->getThirdVertex(a, u);

    /* Get link types */
    bool lAB = first->isTimelike(a, u);
    bool lAC = first->isTimelike(a, c);
    bool lAD = second->isTimelike(a, d);

    Vertex::getAdjacentTriangles(u, b, &third, &fourth);

    bool lCB, lBD;
    if (third->containsVertex(c)) { // order could be turned around, so check!
        lCB = third->isTimelike(b, c);
        lBD = fourth->isTimelike(b, d);
    } else {
        lBD = third->isTimelike(b, d);
        lCB = fourth->isTimelike(b, c);
    }

    first->removeVertices();
    second->removeVertices();
    third->removeVertices();
    fourth->removeVertices();
    delete first;
    delete second;
    delete third;
    delete fourth;
    delete u;

    new Triangle(a, c, b, lAC, lCB, lAB);
    new Triangle(a, b, d, lAB, lBD, lAD);

    BOOST_ASSERT(a->checkCausality());
    BOOST_ASSERT(b->checkCausality());
    BOOST_ASSERT(c->checkCausality());
    BOOST_ASSERT(d->checkCausality());

    return a;
}

Vertex* Moves::doAlexanderAndInverse(Vertex* a, Vertex* b) {
    Triangle* first, *second;
    Vertex::getAdjacentTriangles(a, b, &first, &second);
    Vertex* c = first->getThirdVertex(a, b);
    Vertex* d = second->getThirdVertex(a, b);

    Vertex* u = doAlexander(a, b);
    return doInverseAlexander(a, b, u);
}

Vertex* Moves::doCollapseAndInverse(Vertex* a, Vertex* b) {
    Triangle* first, *second;
    Vertex::getAdjacentTriangles(a, b, &first, &second);
    Vertex* c = first->getThirdVertex(a, b);
    Vertex* d = second->getThirdVertex(a, b);

    b = doCollapse(a, b);
    return doInverseCollapse(c, b, d);
}

Vertex* Moves::doInverseCollapse(Vertex* a, Vertex* b, Vertex* c) {
    Triangle* first, *second;
    Vertex::getAdjacentTriangles(a, b, &first, &second);

    bool lAB = first->isTimelike(a, b);
    Vertex::getAdjacentTriangles(b, c, &first, &second);
    bool lBC = first->isTimelike(b, c); // not really required

    /* Collected all triangles on one side of the barrier a - b - c*/
    TriSet tri;
    Triangle* cur = first;
    Vertex* edgeVertex = cur->getThirdVertex(b, c);

    do {
        tri.insert(cur);
        Vertex::getAdjacentTriangles(edgeVertex, b, &first, &second);
        cur = first == cur ? second : first;
        edgeVertex = cur->getThirdVertex(edgeVertex, b);
    } while (edgeVertex != a); // TODO: check if it collects all

    /* Create new vertex */
    b->getTriangles() -= tri;
    Vertex* u = new Vertex();
    u->getTriangles() += tri;

    foreach(Triangle* t, tri) {
        t->replaceVertex(b, u);
    }

    /* Create two new triangles */
    new Triangle(a, u, b, lAB, !lAB, lAB);
    new Triangle(c, u, b, lAB, !lAB, lAB);

    BOOST_ASSERT(b->checkCausality());
    BOOST_ASSERT(u->checkCausality());

    return b;
}

Vertex* Moves::doMove(Vertex* a, Vertex* b, MOVES move) {
    switch (move) {
        case COLLAPSE_TIMELIKE:
        case COLLAPSE_SPACELIKE:
            return doCollapse(a, b);
        case FLIP:
            return doFlip(a, b);
        case FLIP_CHANGE:
            return doFlipChange(a, b);
        case ALEXANDER_SPACELIKE:
        case ALEXANDER_TIMELIKE:
            return doAlexander(a, b);
    };

    return NULL;
}

int Moves::getInverseCollapseMoveCount(Vertex* u, Vertex* v) {
    int count = 0;

    /* Find out if collapse is spatial */
    Triangle* first, *second;
    Vertex::getAdjacentTriangles(u, v, &first, &second);
    Vertex* c = first->getThirdVertex(u, v);
    Vertex* d = second->getThirdVertex(u, v);

    bool isTimeLike = first->isTimelike(u, v);

    /* Count all relevant links */
    Triangle* cur = first;
    Vertex* edgeVertex = c;

    while (edgeVertex != d) {
        Vertex::getAdjacentTriangles(edgeVertex, u, &first, &second);
        cur = first == cur ? second : first;
        edgeVertex = cur->getThirdVertex(edgeVertex, u);

        if (cur->isTimelike(u, edgeVertex) != isTimeLike) {
            count++;
        }
    }

    /* Count all relevant links */
    Vertex::getAdjacentTriangles(u, v, &first, &second);
    cur = first;
    edgeVertex = c;

    while (edgeVertex != d) {
        Vertex::getAdjacentTriangles(edgeVertex, v, &first, &second);
        cur = first == cur ? second : first;
        edgeVertex = cur->getThirdVertex(edgeVertex, v);

        if (cur->isTimelike(v, edgeVertex) != isTimeLike) {
            count++;
        }
    }

    return (count) * (count - 1) / 2;
}

