/* 
 * File:   Simulation.h
 * Author: ben
 *
 * Created on September 21, 2012, 11:18 AM
 */

#ifndef SIMULATION_H
#define	SIMULATION_H

#include <boost/random/mersenne_twister.hpp>
#include "Triangle.h"

class Simulation {
public:

    enum MOVES {
        ALEXANDER, FLIP, COLLAPSE
    };

    Simulation();
    Simulation(const Simulation& orig);
    virtual ~Simulation();

    /**
     * Generates a random triangulation that satisfies causality.
     * @param N Number of triangles
     * @return Triangulation
     */
    Triangle* generateRandomTriangulation(int N);

    /**
     * Perform Metropolis algorithm to improve a given triangulation.
     * 
     * @param triangulation A single triangle from a triangulation. Since the space is 
     * connected, every other triangle can be reached.
     * 
     * @return Improved triangulation
     */
    Triangle* Metropolis(Triangle* triangulation);
private:
    static const int SEED = 1289730123;
    boost::mt19937 rng;
};

#endif	/* SIMULATION_H */

