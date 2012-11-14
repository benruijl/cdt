/* 
 * File:   main.cpp
 * Author: ben
 *
 * Created on September 19, 2012, 3:01 PM
 */

#include <cstdlib>
#include <iostream>

#include "Simulation.h"
#include "observables/SizeObservable.h"

using namespace std;

int main(int argc, char** argv) {
    Simulation simulation;
    SizeObservable sizeObservable(1, 0); // measure every sweep
    simulation.addObservable(&sizeObservable);

    // read in a thermalized triangulation
    //simulation.readFromFile("grid.dat");

    simulation.generateInitialTriangulation(20, 20);    
    simulation.Metropolis(1.5, -1, 200, 1000000); // 18100 should run for 12 hours

    //simulation.writeToFile("grid.dat");
    simulation.clearTriangulation();

    std::cout << "Simulation ended." << std::endl;

    return 0;
}

