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
    Vertex a, b, c, d, e;
    Triangle t(Triangle::TTS, c, b, a);
    Triangle u(Triangle::TTS, a, d, c);
    Triangle v(Triangle::TTS, e, d, a);
    Triangle w(Triangle::TTS, a, b, e);
    Triangle::registerNeighbour(&t, &u);
    Triangle::registerNeighbour(&u, &v);
    Triangle::registerNeighbour(&v, &w);
    Triangle::registerNeighbour(&w, &t);
    
    std::cout << "Vertex A is causal: " << a.checkCausality() << std::endl;
    std::cout << "Vertex B is causal: " << b.checkCausality() << std::endl;
    
    Simulation simulation;
    Triangle* triangulation = simulation.generateRandomTriangulation(10);
    simulation.Metropolis(triangulation);

    return 0;
}

