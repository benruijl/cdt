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

void Vertex::getAdjacentTriangles(Vertex* a, Vertex* b, Triangle** first, Triangle** second) {
    TriSet t = a->triangles & b->triangles; // intersection
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

bool Vertex::checkCausality() {
    int lightConeCount = 0;
    std::cout << triangles.size() << std::endl;

    foreach(Triangle* t, triangles) {
        lightConeCount += t->getLightConeCount(this);
    }

    std::cout << "Lightcone count: " << lightConeCount << std::endl;
    return lightConeCount == 4;
}

