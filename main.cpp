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

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    Vertex a, b, c, d, e, f;
    Triangle t(a, b, c);
    Triangle u(a, c, d);
    Triangle v(a, d, e);
    Triangle w(a, f, b);
    Triangle::registerNeighbour(&t, &u);
    Triangle::registerNeighbour(&u, &v);
    Triangle::registerNeighbour(&v, &w);
    Triangle::registerNeighbour(&w, &t);

    std::cout << "Vertex A is causal: " << a.checkCausality();
    return 0;
}

