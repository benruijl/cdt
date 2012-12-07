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
#include "observables/GridObservable.h"
#include "observables/TimeSliceObservable.h"

using namespace std;

int main(int argc, char** argv) {
    Simulation simulation;
    SizeObservable sizeObservable(1, 0); // measure every sweep
    GridObservable gridObservable(simulation, 100);
    TimeSliceObservable timeSliceObservable(1);
    simulation.addObservable(&sizeObservable);
    simulation.addObservable(&gridObservable);
    simulation.addObservable(&timeSliceObservable);

    //simulation.readFromFile("grid.dat"); // read in a thermalized triangulation
    simulation.generateInitialTriangulation(25, 25);
    simulation.Metropolis(6, -1, 100, 1000000); // 18100 should run for 12 hours
    std::cout << "Simulation ended." << std::endl;

    return 0;
}

