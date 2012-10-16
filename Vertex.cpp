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

void Vertex::getAdjacentTriangles(Vertex* a, Vertex* b, Triangle** first, Triangle** second) {
    TriSet t = a->triangles & b->triangles; // intersection

    // NOTE: if this assertion fails it is mostly due to the fact that the grid
    // has more than 14 triangles.
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

VertSet Vertex::getSectorVertices(Vertex* v, Vertex* edge, bool left, bool tl) {
    Triangle* cur = NULL, *t, *r;
    Vertex::getAdjacentTriangles(v, edge, &t, &r);
    cur = left ? t : r;

    // always move to first sector transition
    while (cur->getLightConeCount(v) == 0 || cur->isTimelike(v, edge) != tl) {
        Vertex::getAdjacentTriangles(v, edge, &t, &r);
        cur = t == cur ? r : t;
        edge = cur->getThirdVertex(v, edge);
    }

    VertSet vertices;
    while (cur->isTimelike(v, edge) == tl) {
        vertices.insert(edge);
        Vertex::getAdjacentTriangles(v, edge, &t, &r);
        cur = t == cur ? r : t;
        edge = cur->getThirdVertex(v, edge);
    }

    return vertices;
}

bool Vertex::checkCausality() {
    int lightConeCount = 0;

    foreach(Triangle* t, triangles) {
        lightConeCount += t->getLightConeCount(this);
    }

    //std::cout << "Lightcone count: " << lightConeCount << std::endl;
    return lightConeCount == 4;
}

