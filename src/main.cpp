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
#include "moves/MoveFactory.h"
#include "observables/VolumeProfileObservable.h"

using namespace std;

int main(int argc, char** argv) {
    Simulation simulation;
    
    /* Select the moves used in the simulation */
    simulation.getMoveFactory().addAllMoves();
    
    SizeObservable sizeObservable(1, 0); // measure every sweep
    GridObservable gridObservable(simulation, 100);
    TimeSliceObservable timeSliceObservable(10);
    VolumeProfileObservable volumeProfileObservable(10, &timeSliceObservable,
            &simulation);
    simulation.addObservable(&sizeObservable);
    simulation.addObservable(&gridObservable);
    simulation.addObservable(&timeSliceObservable);
    simulation.addObservable(&volumeProfileObservable);

    //simulation.readFromFile("grid.dat"); // read in a thermalized triangulation    
    simulation.generateInitialTriangulation(30, 30);
    simulation.Metropolis(-3.9, 1001, 0.01, 100, 1000000); // 18100 should run for 12 hours
    std::cout << "Simulation ended." << std::endl;

    return 0;
}

