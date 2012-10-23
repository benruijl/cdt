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

#include <vector>
#include "Triangle.h"
#include "Observable.h"

class Simulation {
private:
    std::vector<Observable*> observables;
public:
    Simulation();
    Simulation(const Simulation& orig);
    virtual ~Simulation();
    
    void addObservable(Observable* obs) {
        observables.push_back(obs);
    }

    /**
     * Generates a triangulation that satisfies causality and CDT foliation constraints. 
     * Thus, this initial configuration can be used for both CDT and extended CDT.
     * 
     * The triangulation has the shape of a torus, since both time and space have periodic boundary conditions. 
     * This means that at least 14 triangles have to be generated.
     * 
     * @param N Number of vertices in the circular spatial dimension
     * @param T Number of time slices. Has to be more than 1.
     */
    void generateInitialTriangulation(int N, int T);

    /**
     * Perform Metropolis algorithm to improve a given triangulation. The triangulation
     * should be created with the generateInitialTriangulation method.
     * 
     * @param lambda Action parameter. Controls system size.
     * @param alpha Ratio between length of spacelike and timelike links. 
     * The following should hold: 1/4 &lt; &alpha; &lt; 4 in order for the Wick rotation
     * to be possible.
     * @param numIter Number of iterations
     * 
     * @return New set of vertices that span the triangulation
     */
    VertSet Metropolis(double lambda, double alpha, int numIter);

    /**
     * Convenience function that 
     * @param vertices
     * @return 
     */
    Vertex* getRandomVertex(const VertSet& vertices);

    VertSet& getVertices() {
        return vertices;
    }

    /**
     * Generates a uniform random double in the closed interval [0,1].
     * @return Random number
     */
    double getRandomNumber() {
        return unireal(rng);
    }

    /**
     * Collect triangles around a given vertex that are at most depth links away.
     * @param v
     * @param triSet Set to put triangles in
     * @param depth
     */
    void collectTriangles(TriSet& triSet, Vertex* v, int depth);

    /**
     * Outputs a partial of the triangulation to the 'dot' format.
     * This can be printed with `sfdp graph.dot -Tpdf -o graph.pdf` for example.
     * @param filename
     * @param v Highlight this vertex
     * @param tri Set of triangulation
     */
    void drawPartialTriangulation(const char* filename, Vertex* v, const TriSet& tri);

    /**
     * Selects a random element from a set.
     * @param set
     * @return 
     */
    template <typename T>
    T getRandomElementFromSet(const boost::unordered_set<T>& set);
    
    /**
     * Free the complete triangulation.
     */
    void clearTriangulation();
private:
    static const int SEED = 1289730123;
    boost::mt19937 rng;
    boost::uniform_real<> unireal;

    VertSet vertices; // a list of all the vertices in the simulation
};

#endif	/* SIMULATION_H */

