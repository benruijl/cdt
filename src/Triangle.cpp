        /* 
 * File:   Triangle.cpp
 * Author: ben
 * 
 * Created on September 19, 2012, 3:31 PM
 */

#include "Triangle.h"
#include "Vertex.h"

Triangle::Triangle(TYPE type) {
    this->type = type;
}

Triangle::Triangle(const Triangle& orig) {
    this->type = orig.type;
    vertices[0] = orig.vertices[0];
    vertices[1] = orig.vertices[1];
    vertices[2] = orig.vertices[2];
}

Triangle::Triangle(TYPE type, Vertex* a, Vertex* b, Vertex* c) {
    initialize(type, a, b, c);
}

Triangle::Triangle(Vertex* a, Vertex* b, Vertex* c, bool timeLikeAB, bool timeLikeBC, bool timeLikeCA) {
    if (timeLikeAB && timeLikeBC && !timeLikeCA) {
        initialize(TTS, a, b, c);
        return;
    }

    if (timeLikeBC && timeLikeCA && !timeLikeAB) {
        initialize(TTS, b, c, a);
        return;
    }

    if (timeLikeCA && timeLikeAB && !timeLikeBC) {
        initialize(TTS, c, a, b);
        return;
    }

    if (!timeLikeAB && !timeLikeBC && timeLikeCA) {
        initialize(SST, a, b, c);
        return;
    }

    if (!timeLikeBC && !timeLikeCA && timeLikeAB) {
        initialize(SST, b, c, a);
        return;
    }

    if (!timeLikeCA && !timeLikeAB && timeLikeBC) {
        initialize(SST, c, a, b);
        return;
    }

    BOOST_ASSERT(false);
}

Triangle::~Triangle() {
}

void Triangle::initialize(TYPE type, Vertex* a, Vertex* b, Vertex* c) {
    this->type = type;
    vertices[0] = a;
    vertices[1] = b;
    vertices[2] = c;

    a->registerTriangle(this);
    b->registerTriangle(this);
    c->registerTriangle(this);
}

int Triangle::getLink(Vertex* a, Vertex* b) {
    int i = indexFromVertex(a);
    if (vertices[(i + 1) % 3] == b) {
        return i;
    }

    return (i + 2) % 3;
}

int Triangle::getOppositeLink(Vertex* a) {
    int i = indexFromVertex(a);
    return (i + 1) % 3;
}

bool Triangle::isOppositeLinkTimelike(Vertex* a) {
    return isTimelike(getOppositeLink(a));
}

int Triangle::getAdjacentTimelikeCount(Vertex* v) {
    int i = indexFromVertex(v);
    return isTimelike(i) + isTimelike((i + 2) % 3);
}

bool Triangle::isTimelike(Vertex* a, Vertex* b) {
    int link = getLink(a, b);
    return isTimelike(link);
}

bool Triangle::checkAdjacentSides(Vertex* a, Vertex* b) {
    int link = getLink(a, b);

    return isTimelike((link + 1) % 3) == isTimelike((link + 2) % 3);
}

bool Triangle::checkAdjacentSides(Vertex* v) {
    int i = indexFromVertex(v);
    return isTimelike(i) == isTimelike((i + 2) % 3);
}

Vertex* Triangle::getThirdVertex(Vertex* a, Vertex* b) {
    int i = indexFromVertex(a);
    if (vertices[(i + 1) % 3] == b) {
        return vertices[(i + 2) % 3];
    }

    return vertices[(i + 1) % 3];
}

bool Triangle::containsVertex(Vertex* a) {
    for (int i = 0; i < 3; i++) {
        if (vertices[i] == a) {
            return true;
        }
    }
    return false;
}

int Triangle::getLightConeCount(Vertex* v) {
    int i = indexFromVertex(v);
    return i != 1;
}

void Triangle::replaceVertex(Vertex* old, Vertex* n) {
    int i = indexFromVertex(old);
    vertices[i] = n;
}

bool Triangle::isNeighbour(Triangle* t) {
    int count = 0;

    for (int i = 0; i < 3; i++) {
        count += vertices[i]->getTriangles().find(t) !=
                vertices[i]->getTriangles().end();
    }

    return count == 2;
}

Triangle* Triangle::getNeighbour(const Vertex* u, const Vertex* v) {
    Triangle* q, *r;
    Vertex::getAdjacentTriangles(u, v, &q, &r);
    q = q == this ? r : q;
    return q;
}

void Triangle::removeVertices() {
    for (int i = 0; i < 3; i++) {
        vertices[i]->getTriangles().erase(this);
    }
}

Vertex* Triangle::getNextVertex(Vertex* v) {
    int i = indexFromVertex(v);
    return vertices[(i + 1) % 3];
}