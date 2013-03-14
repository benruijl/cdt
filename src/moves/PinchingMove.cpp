#include "moves/PinchingMove.h" 

PinchingMove::PinchingMove(bool isTimelike) : Move(isTimelike * -4 + !isTimelike * 2,
!isTimelike * -4 + isTimelike * 2) {
    this->isTimelike = isTimelike;
}

double PinchingMove::getTransitionProbability(std::vector<Vertex*>& vertices) {
    // Note that this move is not symmetric, (u,v) and (v, u) are different moves.
    return 1.0 / (vertices.size() * u->getNeighbouringVertexCount());
}

bool PinchingMove::isMovePossible(std::vector<Vertex*>& vertices) {
    Triangle *first, *second, *r, *t;
    Vertex::getAdjacentTriangles(u, v, &first, &second);
    Vertex* w = first->getThirdVertex(u, v);
    Vertex* x = second->getThirdVertex(u, v);

    if (first->isTimelike(u, v) != isTimelike || first->isTimelike(u, w) == isTimelike || second->isTimelike(u, x) == isTimelike
            || first->isTimelike(v, w) != isTimelike || second->isTimelike(v, x) != isTimelike) {
        return false;
    }

    r = first->getNeighbour(v, w);
    Vertex* y = r->getThirdVertex(v, w);

    if (r->isTimelike(v, y) == isTimelike || r->isTimelike(w, y) != isTimelike) {
        return false;
    }

    t = second->getNeighbour(v, x);
    Vertex* z = t->getThirdVertex(v, x);

    if (t->isTimelike(v, z) == isTimelike || t->isTimelike(x, z) != isTimelike) {
        return false;
    }

    /* A fixed triangle should not be deleted. */
    if (getFixedTriangle() == t || getFixedTriangle() == r ||
            getFixedTriangle() == first || getFixedTriangle() == second) {
        return false;
    }

    // prevent link overlap
    VertSet verts = v->getNeighbouringVertices();
    return (verts & neighboursU).size() == 2;
}

Move* PinchingMove::generateRandomMove(Simulation& simulation) {
    u = simulation.getRandomVertex(simulation.getVertices());
    neighboursU = u->getNeighbouringVertices();
    v = simulation.getRandomElementFromSet(neighboursU);
    return this;
}

double PinchingMove::getInverseTransitionProbability(std::vector<Vertex*>& vertices) {
    Triangle* l, *r;
    Vertex::getAdjacentTriangles(u, v, &l, &r);

    // count the number of triangles that belong in the other
    // sector after this move is done
    int countLeft = u->getSectorVertices(l, true, !isTimelike).size() - 1;
    int countRight = u->getSectorVertices(r, false, !isTimelike).size() - 1;

    Vertex* w = l->getThirdVertex(u, v);
    Vertex* x = r->getThirdVertex(u, v);

    countLeft += v->getSectorVertices(l, w, !isTimelike).size() - 1;
    countRight += v->getSectorVertices(r, x, !isTimelike).size() - 1;

    return 1.0 / ((countLeft + 1) * (countRight + 1) * 2);
}

void PinchingMove::execute(std::vector<Vertex*>& vertices) {
    Triangle *first, *second, *third, *fourth, *r, *t;
    Vertex::getAdjacentTriangles(u, v, &first, &second);
    Vertex* w = first->getThirdVertex(u, v);
    Vertex* x = second->getThirdVertex(u, v);

    Vertex::getAdjacentTriangles(v, w, &r, &t);
    third = r == first ? t : r;
    Vertex* y = third->getThirdVertex(v, w);

    Vertex::getAdjacentTriangles(v, x, &r, &t);
    fourth = r == second ? t : r;
    Vertex* z = fourth->getThirdVertex(v, x);

    /* Clean-up */
    first->removeVertices();
    second->removeVertices();
    third->removeVertices();
    fourth->removeVertices();
    vertices.erase(std::remove(vertices.begin(), vertices.end(), v), vertices.end());

    /* Relabel */
    foreach(Triangle* t, v->getTriangles()) {
        t->replaceVertex(v, u);
        u->getTriangles().insert(t);
    }

    /* Create new triangles */
    new Triangle(w, u, y, !isTimelike, !isTimelike, isTimelike);
    new Triangle(x, u, z, !isTimelike, !isTimelike, isTimelike);

    delete first;
    delete second;
    delete third;
    delete fourth;
    delete v;

    BOOST_ASSERT(u->checkCausality());
    BOOST_ASSERT(w->checkCausality());
    BOOST_ASSERT(x->checkCausality());
    BOOST_ASSERT(y->checkCausality());
    BOOST_ASSERT(z->checkCausality());
}

std::string PinchingMove::printID() {
    return std::string("PINCH_") + (isTimelike ? "TL" : "SL");
}
