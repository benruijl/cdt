/* 
 * File:   Vertex.cpp
 * Author: ben
 * 
 * Created on September 19, 2012, 3:31 PM
 */

#include "Vertex.h"
#include "Triangle.h"

Vertex::Vertex() {
    t = 0;
    x = 0;
    triangle = NULL;
}

Vertex::Vertex(const Vertex& orig) {
}

Vertex::~Vertex() {
}

bool Vertex::checkCausality() {
    int lightConeCount = 0;

    /* Find neighbours */
    Triangle* cur = triangle;
    while ((cur = cur->getNeighbourClockwise(this)) != triangle) {
        std::cout << "Neighbour passed!";

    }
    
    return false;
}

