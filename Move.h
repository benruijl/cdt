/* 
 * File:   Move.h
 * Author: bruijl
 *
 * Created on October 9, 2012, 1:27 PM
 */

#ifndef MOVE_H
#define	MOVE_H

#include "Vertex.h"
#include <vector>

class Simulation;

class Move {
public:
    /**
     * Creates a new move.
     * @param Ntts Number of TTS triangles this move changes. Can be negative.
     * @param Ntss Number of TSS triangles this move changes. Can be negative.
     * @return New move
     */
    Move(int Ntts, int Ntss);
    virtual ~Move();

    /**
     * Gets the Boltzmann probability of a move, i.e. P(x')/P(x)
     * @param lambda Action parameter
     * @param alpha Ratio between timelike and spacelike lengths
     * @return P(x')/P(x)
     */
    double getMoveProbability(double lambda, double alpha);
    
    /**
     * Gets the probability of performing this particular move.
     * @param vertices Set of vertices
     * @return 
     */
    virtual double getTransitionProbability(std::vector<Vertex*>& vertices) = 0;

    /**
     * Gets the transition probability Q(x | x') after a move is done from x to
     * x'. This uses the <b>current</b> triangulation x, and the move information
     * to get from x to x'.
     * @param vertices Set of vertices
     * @return Q(x | x')
     */
    virtual double getInverseTransitionProbability(std::vector<Vertex*>& vertices) = 0;

    /**
     * Completes this move by generating enough vertices to do the move. The move
     * could be invalid and has to be checked with isMovePossible.
     * @param simulation The current simulation
     * @return This move
     */
    virtual Move* generateRandomMove(Simulation& simulation) = 0;
    
    /**
     * Checks if this move is possible.
     * @param vertices Set of vertices
     * @return True if possible, else false
     */
    virtual bool isMovePossible(std::vector<Vertex*>& vertices) = 0;

    /**
     * Performs the move on the grid of vertices.
     * @param vertices Set of vertices
     */
    virtual void execute(std::vector<Vertex*>& vertices) = 0;
    
    /**
     * Prints the move id. Useful for debugging.
     */
    virtual std::string printID() = 0;
private:
    int Ntts, Ntss; // Number of TTS and TSS triangles this move changes
};

#endif	/* MOVE_H */

