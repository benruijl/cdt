#include "moves/InversePinchingMove.h"

InversePinchingMove::InversePinchingMove(bool isTimelike) : Move(isTimelike * 4 + !isTimelike * -2,
!isTimelike * 4 + isTimelike * -2) {
    t = NULL;
    s = NULL;
    this->isTimelike = isTimelike;
}

double InversePinchingMove::getTransitionProbability(std::vector<Vertex*>& vertices) {
    Triangle* start = *v->getTriangles().begin();
    TriSet triRight = v->getSectorTriangles(start, false, !isTimelike);
    TriSet triLeft = v->getSectorTriangles(start, true, !isTimelike);

    return 1.0 / (vertices.size() * triRight.size() * triLeft.size() * 2);
}

bool InversePinchingMove::isMovePossible(std::vector<Vertex*>& vertices) {
    return t != NULL && s != NULL && getFixedTriangle() != t &&
            getFixedTriangle() != s;
}

Move* InversePinchingMove::generateRandomMove(Simulation& simulation) {
    v = simulation.getRandomVertex(simulation.getVertices());
    Triangle* start = *v->getTriangles().begin();

    TriSet tri = v->getSectorTriangles(start, false, !isTimelike);
    t = simulation.getRandomElementFromSet(tri);
    tri = v->getSectorTriangles(start, true, !isTimelike);
    s = simulation.getRandomElementFromSet(tri);

    left = simulation.getRandomNumber() < 0.5;
    return this;
}

double InversePinchingMove::getInverseTransitionProbability(std::vector<Vertex*>& vertices) {
    /* TODO: repetition of code, same thing done in execute as well*/
    Vertex* u = t->getNextVertex(v);
    Vertex* y = s->getNextVertex(v);
    Vertex* z = s->getNextVertex(y);

    if (left) {
        u = t->getNextVertex(u);
    }

    int count = 1; // last vertex is not counted, so add it here
    Triangle* cur = t;
    Vertex* edge = u;

    while (edge != y && edge != z) {
        cur = cur->getNeighbour(v, edge);
        edge = cur->getThirdVertex(v, edge);
        count++;
    }

    // count + 1 because there is a new vertex
    return 1.0 / ((vertices.size() + 1) * (count + 2));
}

void InversePinchingMove::execute(std::vector<Vertex*>& vertices) {
    Vertex* u = t->getNextVertex(v);
    Vertex* y = s->getNextVertex(v);
    Vertex* z = s->getNextVertex(y);

    if (left) {
        u = t->getNextVertex(u);
    }

    TriSet tri;
    Triangle* cur = t;
    Vertex* edge = u;

    while (edge != y && edge != z) {
        cur = cur->getNeighbour(v, edge);
        edge = cur->getThirdVertex(v, edge);
        tri.insert(cur);
    }

    // the three vertices are now u -- v -- edge
    Vertex* w = t->getThirdVertex(v, u);
    z = s->getThirdVertex(v, edge);

    t->removeFromVertices();
    s->removeFromVertices();
    delete t;
    delete s;

    Vertex* x = new Vertex();
    vertices.push_back(x);

    x->getTriangles() += v->getTriangles();
    x->getTriangles() -= tri;
    v->getTriangles() -= x->getTriangles();

    foreach(Triangle* a, x->getTriangles()) {
        a->replaceVertex(v, x);
    }

    new Triangle(u, x, v, isTimelike, isTimelike, !isTimelike);
    new Triangle(v, x, edge, isTimelike, isTimelike, !isTimelike);
    new Triangle(w, u, x, isTimelike, isTimelike, !isTimelike);
    new Triangle(x, edge, z, isTimelike, isTimelike, !isTimelike);

    BOOST_ASSERT(v->checkCausality());
    BOOST_ASSERT(x->checkCausality());
    BOOST_ASSERT(w->checkCausality());
    BOOST_ASSERT(z->checkCausality());
    BOOST_ASSERT(u->checkCausality());
    BOOST_ASSERT(edge->checkCausality());
}

std::string InversePinchingMove::printID() {
    return std::string("INV_PINCH_") + (isTimelike ? "TL" : "SL");
}
