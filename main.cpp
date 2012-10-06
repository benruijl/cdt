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
#include "Moves.h"

using namespace std;

int main(int argc, char** argv) {
    Simulation simulation;

    simulation.generateInitialTriangulation(4, 4);
    simulation.Metropolis(1, 1);

    return 0;
}

