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
     * Performs a move on the link spanned by vertex a and b.
     */
    Vertex* doMove(Vertex* a, Vertex* b, MOVES move);


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
     * 
     * @return Improved triangulation
     */
    Triangle* Metropolis(Triangle* triangulation);
private:
    static const int SEED = 1289730123;
    boost::mt19937 rng;

    /**
            Do the collapse move. It removes all the occurrences of a and replaces it with b.
            TODO: move to separate Moves class
     */
    Vertex* doCollapse(Vertex* a, Vertex* b);
    
    Vertex* doFlip(Vertex* a, Vertex* b);
};

#endif	/* SIMULATION_H */

