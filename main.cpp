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

/*
 * 
 */
int main(int argc, char** argv) {
    Simulation simulation;
    Triangle* triangulation = simulation.generateInitialTriangulation(2, 2);

	// Test a collapse:
	simulation.doMove(triangulation->getVertex(0), triangulation->getVertex(1), Simulation::COLLAPSE);

    simulation.Metropolis(triangulation);

    return 0;
}

