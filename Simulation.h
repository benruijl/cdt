/* 
 * File:   Simulation.h
 * Author: ben
 *
 * Created on September 21, 2012, 11:18 AM
 */

#ifndef SIMULATION_H
#define	SIMULATION_H

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real.hpp>

#include "Triangle.h"
#include "Moves.h"

class Simulation {
public:
    Simulation();
    Simulation(const Simulation& orig);
    virtual ~Simulation();
    
    /**
     * Gives the number of moves that are possible at a given vertex.
     * @param v Vertex v
     * @return
     */
    int getAcceptableMoveCount(Vertex* v);
            
    /**
     * Generates a triangulation that satisfies causality and CDT foliation constraints. 
     * Thus, this initial configuration can be used for both CDT and extended CDT.
     * 
     * The triangulation has the shape of a torus, since both time and space have periodic boundary conditions. 
     * This means that at least 14 triangles have to be generated.
     * 
     * @param N Number of vertices in the circular spatial dimension
     * @param T Number of time slices. Has to be more than 1.
     * @return Triangulation
     */
    Triangle* generateInitialTriangulation(int N, int T);

    /**
     * Perform Metropolis algorithm to improve a given triangulation.
     * 
     * @param triangulation A single triangle from a triangulation. Since the space is 
     * connected, every other triangle can be reached.
     * @param lambda Action parameter
     * @param alpha Ratio between length of spacelike and timelike links. should be positive
     * 
     * @return Improved triangulation
     */
    Vertex* Metropolis(Triangle* triangulation, double lambda, double alpha);
private:
    static const int SEED = 1289730123;
    boost::mt19937 rng;
    boost::uniform_real<> unireal;
    
    Vertex* vertices; // a list of all the vertices in the simulation
};

#endif	/* SIMULATION_H */

