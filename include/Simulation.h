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
#include <boost/random/uniform_int.hpp>
#include <vector>

#include "Vertex.h"
#include "Triangle.h"

class Observable;
class MoveFactory;

typedef boost::mt19937 base_generator_type; // our RNG

class Simulation {
private:
    std::vector<Observable*> observables;
    MoveFactory* moveFactory;
public:
    Simulation();
    Simulation(const Simulation& orig);
    virtual ~Simulation();

    /**
     * Adds an observable to the simulator. The simulator will free the memory.
     * @param obs Pointer to an observable.
     */
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
     * The lambda term in the action is tuned in this method, so that the average
     * volume is `volume`.
     * 
     * @param alpha Ratio between length of spacelike and timelike links. 
     * The following should hold: 1/4 &lt; &alpha; &lt; 4 in order for the Wick rotation
     * to be possible.
     * @param volume Desired number of triangles. This adds a volume fixing term
     * that allows for small fluctuations.
     * @param deltaVolume Fluctuation parameter for fixed volume range
     * @param numSweeps Number of sweeps
     * @param sweepLength How many iterations should a sweep be?
     * 
     */
    void Metropolis(double alpha, unsigned int volume, double
            deltaVolume, unsigned int numSweeps, unsigned int sweepLength);

    /**
     * Convenience function that 
     * @param vertices
     * @return 
     */
    Vertex* getRandomVertex(const std::vector<Vertex*>& vertices);

    std::vector<Vertex*>& getVertices() {
        return vertices;
    }

    /**
     * Generates a uniform random double in the closed interval [0,1].
     * @return Random number
     */
    double getRandomNumber() {
        return unireal(rng);
    }

    void checkLinkOverlap();

    /**
     * Creates a unique ID for the current triangulation.
     * @param t Starting triangle. In order to compare IDs, the simulation should
     * not delete this triangle.
     * 
     * TODO: make bytestream?
     */
    std::vector<int> createID(Triangle* t);

    /**
     * Perform a simple Boltzmann test. Counts the frequency of the lowest energy
     * states.
     */
    void doBoltmannTest();

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
     * Prints which triangles are connected to which three others.
     * @param tri Triangle set
     */
    void printTriangleConnectivity(Triangle* t);

    /**
     * Selects a random element from a set.
     * @param set
     * @return 
     */
    template <typename T>
    T getRandomElementFromSet(const boost::unordered_set<T>& set) {
        typename boost::unordered_set<T>::iterator it = set.begin();

        if (set.size() == 0) {
            return NULL; // TODO: add assert?
        }

        boost::uniform_int<> uint(0, set.size() - 1); // TODO: check if slow?
        std::advance(it, uint(rng));
        return *it;
    }

    /**
     * Free the complete triangulation.
     */
    void clearTriangulation();

    /**
     * Reads a triangulation from a file.
     */
    void readFromFile(const char* filename);

    /**
     * Writes the triangulation to a human readable file.
     */
    void writeToFile(const char* filename);

    /**
     * Returns the current pseudo random number generator.
     */
    base_generator_type& getRNG() {
        return rng;
    }

    void setSeed(unsigned int seed) {
        //std::cout << "Using seed: " << seed << std::endl;
        rng.seed(seed);
    }

    MoveFactory& getMoveFactory() {
        return *moveFactory;
    }
private:
    base_generator_type rng;
    boost::uniform_real<> unireal;
    std::vector<Vertex*> vertices; // a list of all the vertices in the simulation

    /* Some statistics of the simulation */
    int TTSCount, SSTCount; // number of TTS and SST triangles
    double k, z; // constants for lambda fine-tuning
};

#endif	/* SIMULATION_H */

