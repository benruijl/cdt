/* 
 * File:   Simulation.cpp
 * Author: ben
 * 
 * Created on September 21, 2012, 11:18 AM
 */

#include "Simulation.h"

Simulation::Simulation() {
    /* Initialize random number generator */
    rng.seed(SEED);
}

Simulation::Simulation(const Simulation& orig) {
}

Simulation::~Simulation() {
}

Triangle* Simulation::generateInitialTriangulation(int N, int T) {
    Vertex* vertices[N * T];
    Triangle* triangles[N * T * 2];

    /* Create vertices */
    for (int t = 0; t < T * N; t++) {
        vertices[t] = new Vertex();
    }

    /* Create a foliation */
    for (int t = 0; t < T; t++) {
        for (int s = 0; s < N; s++) {
            Triangle* u = new Triangle(Triangle::TTS, vertices[t * N + s], vertices[((t + 1) % T) * N + s],
                    vertices[t * N + (s + 1) % N]);
            Triangle* v = new Triangle(Triangle::TTS, vertices[t * N + s], vertices[((t - 1 + T) % T) * N + s],
                    vertices[t * N + (s + 1) % N]);
            triangles[t * N + 2 * s] = u;
            triangles[t * N + 2 * s + 1] = v;
        }
    }

    /* Create neighbour list */
    for (int t = 0; t < T; t++) {
        for (int s = 0; s < N; s++) {
            Triangle::registerNeighbour(triangles[t * N + 2 * s], triangles[t * N + 2 * s + 1]);
            Triangle::registerNeighbour(triangles[t * N + 2 * s], triangles[((t + 1) % T) * N + 2 * s + 1]);
        }
    }


    return triangles[0];
}

Triangle * Simulation::Metropolis(Triangle * triangulation) {
    return triangulation;
}

