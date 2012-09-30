/* 
 * File:   main.cpp
 * Author: ben
 *
 * Created on September 19, 2012, 3:01 PM
 */

#include <cstdlib>
#include <iostream>

#include "Vertex.h"
#include "Triangle.h"
#include "Simulation.h"

using namespace std;

int main(int argc, char** argv) {
    Simulation simulation;
    Triangle* triangulation = simulation.generateInitialTriangulation(3, 3);
    
    // Test a collapse, remove a spacelike link
    //simulation.doMove(triangulation->getVertex(2), triangulation->getVertex(0), Simulation::COLLAPSE);
    //simulation.doMove(triangulation->getVertex(2), triangulation->getVertex(1), Simulation::FLIP);
    simulation.doMove(triangulation->getVertex(1), triangulation->getVertex(2), Simulation::ALEXANDER);

    simulation.Metropolis(triangulation);

    return 0;
}

