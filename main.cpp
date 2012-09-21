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

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    Vertex a, b, c, d, e, f;
    Triangle t(Triangle::TSS, a, b, c);
    Triangle u(Triangle::TTS, d, c, a);
    Triangle v(Triangle::TSS, a, e, d);
    Triangle w(Triangle::TTS, b, f, a);
    Triangle::registerNeighbour(&t, &u);
    Triangle::registerNeighbour(&u, &v);
    Triangle::registerNeighbour(&v, &w);
    Triangle::registerNeighbour(&w, &t);
    
    std::cout << "Vertex A is causal: " << a.checkCausality();
    
    Simulation simulation;
    Triangle* triangulation = simulation.generateRandomTriangulation();
    simulation.Metropolis(triangulation);

    return 0;
}

