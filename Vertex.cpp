/* 
 * File:   Vertex.cpp
 * Author: ben
 * 
 * Created on September 19, 2012, 3:31 PM
 */

#include "Vertex.h"
#include "Triangle.h"

Vertex::Vertex() {
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
    do  {
        lightConeCount += cur->getLightConeCount(this);
        cur = cur->getNeighbourClockwise(this);
        
        if (cur == NULL) { /* are we at an edge? */
            std::cout << "Edge detected => always causal" << std::endl;
            return true;
        }
        
    } while(cur != triangle);
    
    std::cout << "Lightcone count: " << lightConeCount << std::endl;
    return lightConeCount == 4;
}

