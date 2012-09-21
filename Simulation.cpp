/* 
 * File:   Simulation.cpp
 * Author: ben
 * 
 * Created on September 21, 2012, 11:18 AM
 */

#include "Simulation.h"

Simulation::Simulation() {
    /* Initialize random number generator */
    rng.seed(SEED);
}

Simulation::Simulation(const Simulation& orig) {
}

Simulation::~Simulation() {
}

Triangle* Simulation::generateRandomTriangulation(int N) {
    Triangle* triangles = new Triangle*[N];
    
    
    return triangles;    
}

Triangle* Simulation::Metropolis(Triangle* triangulation) {
    return triangulation;
}

