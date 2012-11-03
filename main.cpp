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

void getFitOfSystemSize(double lambda, int iter, double& a, double &b) {
    Simulation simulation;
    SizeObservable* sizeObservable = new SizeObservable(100, 200);
    simulation.addObservable(sizeObservable);

    simulation.generateInitialTriangulation(20, 20);
    simulation.Metropolis(lambda, -1, iter);
    simulation.clearTriangulation();

    sizeObservable->printResult("size.dat"); // for debugging
    // FIXME: 200 should be set elsewhere
    return sizeObservable->getLinearFit(200, a, b);
}

void findLamdbaForStableSystemSize() {
    double left = 1, right = 20; // bounds for binary search
    double a, b;
    int n = 10, iter = 2000000; // precision

    for (int i = 0; i < n; i++) {
        double middle = (left + right) / 2.0;
        getFitOfSystemSize(middle, iter, a, b);

        if (b > 0) {
            left = middle;
        } else {
            right = middle;
        }

        std::cout << middle << " " << b << std::endl;
    }
}

int main(int argc, char** argv) {
    //findLamdbaForStableSystemSize();

    Simulation simulation;
    SizeObservable* sizeObservable = new SizeObservable(1000000, 1000000);
    simulation.addObservable(sizeObservable);

    // read in a thermalized triangulation
    //simulation.readFromFile("grid.dat");

    simulation.generateInitialTriangulation(30, 30);
    // at a = -0.5 there should be about the same TTS as SST triangles
    simulation.Metropolis(1.230, -1, 12000000ul);

    sizeObservable->printResult("size.dat"); // for debugging
    //simulation.writeToFile("grid.dat");
    simulation.clearTriangulation();

    std::cout << "Simulation ended." << std::endl;
    return 0;
}

