/* 
 * File:   Vertex.cpp
 * Author: ben
 * 
 * Created on September 19, 2012, 3:31 PM
 */

#include "Vertex.h"
#include "Triangle.h"
#include "Utils.h"

Vertex::Vertex() {
}

Vertex::Vertex(const Vertex& orig) {
}

Vertex::~Vertex() {
}

VertSet Vertex::getNeighbouringVertices() {
    VertSet neighbours;

    foreach(Triangle* t, triangles) {
        for (int i = 0; i < 3; i++) {
            neighbours.insert(t->getVertex(i));
        }
    }

    neighbours.erase(this);
    return neighbours;
}

void Vertex::getAdjacentTriangles(const Vertex* a, const Vertex* b,
        Triangle** first, Triangle** second) {
    TriSet t = a->triangles & b->triangles; // intersection

    // NOTE: if this assertion fails it is mostly due to the fact that the grid
    // has less than 14 triangles.
    if (t.size() != 2) {
        std::cout << "Violated at " << a << " " << b << std::endl;
    }


    BOOST_ASSERT(t.size() == 2); // each link should have 2 triangles

    *first = NULL;

    // TODO: improve

    foreach(Triangle* tr, t) {
        if (*first == NULL) {
            *first = tr;
        } else {
            *second = tr;
            return;
        }
    }
}

VertSet Vertex::getSectorVertices(Triangle* start, Vertex* u, bool tl) {
    Triangle* cur = start;
    Vertex* edge = u;

    // always move to first sector transition
    while (cur->getLightConeCount(this) == 0 || cur->isTimelike(this, edge) != tl) {
        cur = cur->getNeighbour(this, edge);
        edge = cur->getThirdVertex(this, edge);
    }

    VertSet vertices;
    while (cur->isTimelike(this, edge) == tl) {

        vertices.insert(edge);
        cur = cur->getNeighbour(this, edge);
        edge = cur->getThirdVertex(this, edge);
    }

    return vertices;
}

VertSet Vertex::getOtherSectorVertices(Vertex* u) {
    Triangle* t, *r;
    Vertex::getAdjacentTriangles(this, u, &t, &r);
    bool tl = t->isTimelike(this, u);

    // start from the next vertex, to avoid returning u, if u is the only element
    // of its sector
    return getSectorVertices(t, t->getThirdVertex(this, u), t->isTimelike(this, u));
}

VertSet Vertex::getSectorVertices(Triangle* start, bool left, bool tl) {
    Triangle* cur = start;
    Vertex* edge = cur->getNextVertex(this);

    // always move to first sector transition
    while (cur->getLightConeCount(this) == 0 || left || cur->isTimelike(this, edge) != tl) {
        if (cur->getLightConeCount(this) == 1 && cur->isTimelike(this, edge) == tl) {
            left = false; // if left, go to second transition
        }

        cur = cur->getNeighbour(this, edge);
        edge = cur->getThirdVertex(this, edge);
    }

    VertSet vertices;
    while (cur->isTimelike(this, edge) == tl) {
        vertices.insert(edge);
        cur = cur->getNeighbour(this, edge);
        edge = cur->getThirdVertex(this, edge);
    }

    return vertices;
}

TriSet Vertex::getSectorTriangles(Triangle* start, bool left, bool tl) {

    VertSet vert = getSectorVertices(start, left, tl);
    TriSet tri;
    Triangle* t, *r;

    foreach(Vertex* w, vert) {
        Vertex::getAdjacentTriangles(this, w, &t, &r);
        if (t->checkAdjacentSides(this)) {
            tri.insert(t);
        }
        if (r->checkAdjacentSides(this)) {
            tri.insert(r);
        }
    }

    return tri;
}

bool Vertex::checkCausality() {

    int lightConeCount = 0;

    foreach(Triangle* t, triangles) {
        lightConeCount += t->getLightConeCount(this);
    }

    //std::cout << "Lightcone count: " << lightConeCount << std::endl;
    return lightConeCount == 4;
}

