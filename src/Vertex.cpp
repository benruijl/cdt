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

Vertex* Vertex::getNeighbouringVertex() {
    return (*triangles.begin())->getNextVertex(this);
}

VertSet Vertex::getNeighbouringVertices() {
    VertSet neighbours;
    neighbours.rehash(triangles.size() + 1);

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

    // Is the symplectic manifold condition broken?
    if (t.size() != 2) {
        std::cout << "Symplectic manifold broken at " << a << " " << b << std::endl;
    }

    BOOST_ASSERT(t.size() == 2); // each link should have 2 triangles
    
    *first = *t.begin();
    *second = *(++t.begin());
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

    // start from the next vertex, to avoid returning u, if u is the only element
    // of its sector
    VertSet o = getSectorVertices(t, t->getThirdVertex(this, u), t->isTimelike(this, u));

    VertSet v = getSectorVertices(t, true, t->isTimelike(this, u));
    VertSet w = getSectorVertices(t, false, t->isTimelike(this, u));
    BOOST_ASSERT((v & w).size() == 0);
    BOOST_ASSERT(((v & o).size() == 0 && w == o) || ((w & o).size() == 0 && v == o));

    return o;
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

void Vertex::printConnectivity() {
    Triangle* t, *r;
    Vertex* u = (*triangles.begin())->getNextVertex(this);
    Vertex::getAdjacentTriangles(this, u, &t, &r);

    std::cout << this << ": ";
    r = t;

    do {
        std::cout << (t->isTimelike(this, u) ? "T " : "S ") << u << " ";
        t = t->getNeighbour(this, u);
        u = t->getThirdVertex(this, u);
    } while (r != t);

    std::cout << std::endl;
}
