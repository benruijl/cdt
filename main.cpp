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

void getFitOfSystemSize(double lambda, int iter, double& a, double &b) {
    Simulation simulation;
    SizeObservable* sizeObservable = new SizeObservable();
    simulation.addObservable(sizeObservable);

    simulation.generateInitialTriangulation(20, 20);
    simulation.Metropolis(lambda, 1, iter);
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
    SizeObservable* sizeObservable = new SizeObservable();
    simulation.addObservable(sizeObservable);

    simulation.generateInitialTriangulation(20, 20);
    // at 14 the size is kind of stable at around 150 vertices after 680 moves
    // TODO: should it check triangles or vertices?
    simulation.Metropolis(14, 1, 30000000);

    sizeObservable->printResult("size.dat"); // for debugging
    simulation.writeToFile("grid.dat");
    simulation.clearTriangulation();

    std::cout << "Simulation ended." << std::endl;
    return 0;
}

