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
    SizeObservable* sizeObservable = new SizeObservable(1, 0); // measure every sweep
    simulation.addObservable(sizeObservable);

    // read in a thermalized triangulation
    //simulation.readFromFile("grid.dat");

    simulation.generateInitialTriangulation(30, 30);
    simulation.Metropolis(1.225, -1, 10, 1000000);

    simulation.writeToFile("grid.dat");
    simulation.clearTriangulation();

    std::cout << "Simulation ended." << std::endl;
    delete sizeObservable; // make sure destructor is called
    
    return 0;
}

