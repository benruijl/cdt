/* 
 * File:   main.cpp
 * Author: ben
 *
 * Created on September 19, 2012, 3:01 PM
 */

#include <cstdlib>
#include <iostream>

#include "Simulation.h"
#include "SizeObservable.h"

using namespace std;

void systemSizeTest() {
    Simulation simulation;
    SizeObservable* sizeObservable = new SizeObservable();
    simulation.addObservable(sizeObservable);

    simulation.generateInitialTriangulation(20, 20);
    simulation.Metropolis(20, 1, 100000);
    
    sizeObservable->printResult("size.dat");
}

int main(int argc, char** argv) {
    systemSizeTest();

    std::cout << "Simulation ended." << std::endl;
    return 0;
}

