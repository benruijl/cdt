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

void Vertex::getAdjacentTriangles(Vertex* a, Vertex* b, Triangle** first, Triangle** second) {
    *first = *second = NULL;
	Triangle* cur = a->triangle;
    do  {
		if (cur->containsVertex(b)) {
			if (*first == NULL) {
				*first = cur;
			} else {
				*second = cur;
				return;
			}
		}
        cur = cur->getNeighbourClockwise(a);
    } while(cur != a->triangle);

}

bool Vertex::checkCausality() {
    int lightConeCount = 0;

    /* Find neighbours */
    Triangle* cur = triangle;
    do  {
        lightConeCount += cur->getLightConeCount(this);
        cur = cur->getNeighbourClockwise(this);
        
		if (cur == NULL) { /* are we at an edge? */
            std::cerr << "Edge detected => shouldn't happen" << std::endl;
            return false;
        }
        
    } while(cur != triangle);
    
    //std::cout << "Lightcone count: " << lightConeCount << std::endl;
    return lightConeCount == 4;
}

