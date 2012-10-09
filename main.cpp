/* 
 * File:   main.cpp
 * Author: ben
 *
 * Created on September 19, 2012, 3:01 PM
 */

#include <cstdlib>
#include <iostream>

#include "Simulation.h"

using namespace std;

int main(int argc, char** argv) {
    Simulation simulation;
    
    simulation.generateInitialTriangulation(10, 10);
    simulation.Metropolis(1, 1);

    return 0;
}

