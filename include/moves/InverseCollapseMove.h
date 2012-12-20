/* 
 * File:   InverseCollapseMove.h
 * Author: bruijl
 *
 * Created on October 9, 2012, 3:22 PM
 */

#ifndef INVERSECOLLAPSEMOVE_H
#define	INVERSECOLLAPSEMOVE_H

#include "Move.h"

class InverseCollapseMove : public Move {
private:
    Vertex* u, *v, *w; // u is the center triangle
    bool isTimelike;

    /**
     * Gets all the triangles that contain vertex u and that are on either the left
     * or right side of the barrier v - u -w.
     * 
     * TODO: rewrite in terms of sector vertices
     * @param left Check the left side?
     * @return 
     */
    TriSet getBarrierTriangles(bool left);
public:

    InverseCollapseMove(bool timelike);

    double getTransitionProbability(std::vector<Vertex*>& vertices);

    bool isMovePossible(std::vector<Vertex*>& vertices);

    Move* generateRandomMove(Simulation& simulation);

    double getInverseTransitionProbability(std::vector<Vertex*>& vertices);

    void execute(std::vector<Vertex*>& vertices);

    std::string printID();
};


#endif	/* INVERSECOLLAPSEMOVE_H */

