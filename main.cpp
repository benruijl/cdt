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
    Triangle* triangulation = simulation.generateInitialTriangulation(4, 4);
    
    // Test a collapse, remove a spacelike link
    //simulation.doMove(triangulation->getVertex(2), triangulation->getVertex(0), Simulation::COLLAPSE);
    //simulation.doMove(triangulation->getVertex(2), triangulation->getVertex(1), Simulation::FLIP);
    //simulation.doMove(triangulation->getVertex(0), triangulation->getVertex(1), Simulation::FLIP2);
    //simulation.doAlexanderAndInverse(triangulation->getVertex(0), triangulation->getVertex(1));
    
    // NOTE: triangulation is not valid anymore after this move
    //simulation.doCollapseAndInverse(triangulation->getVertex(0), triangulation->getVertex(2));
    
    Moves m;
    std::cout << m.getInverseCollapseMoveCount(triangulation->getVertex(0), triangulation->getVertex(2));

    simulation.Metropolis(triangulation, 1, 1);

    return 0;
}

