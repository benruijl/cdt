/* 
 * File:   Simulation.cpp
 * Author: ben
 * 
 * Created on September 21, 2012, 11:18 AM
 */

#include "Simulation.h"

Simulation::Simulation() {
    /* Initialize random number generator */
    rng.seed(static_cast<boost::mt19937::result_type>(SEED));
}

Simulation::Simulation(const Simulation& orig) {
}

Simulation::~Simulation() {
}

Triangle* Simulation::doCollapse(Vertex* a, Vertex* b) {
	Triangle* tri = a->getTriangle();

	// TODO: check if the side links are of the same type
	// if not, the replacement

	Triangle* first, *second;
	Vertex::getAdjacentTriangles(a, b, &first, &second);
	int linkA = first->getLink(a, b);
	int linkB = second->getLink(a, b);
	
	do {
		tri->replaceVertex(a, b);
		tri = tri->getNeighbourClockwise(b);
	} while (tri != a->getTriangle());
	
	Triangle::registerNeighbour(first->getNeighbour((linkA + 1) % 3), first->getNeighbour((linkA + 2) % 3));
	Triangle::registerNeighbour(second->getNeighbour((linkB + 1) % 3), second->getNeighbour((linkB + 2) % 3));

	if (!b->checkCausality()) {
		std::cerr << "Causality check failed after collapse";
	}

	free(a);
	free(first);
	free(second);

	return b->getTriangle();
}

Triangle* Simulation::doMove(Vertex* a, Vertex* b, MOVES move) {
	switch(move) {
		case COLLAPSE:	
			return doCollapse(a, b);
	};

	return NULL;
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
            Triangle* v = new Triangle(Triangle::TTS, vertices[((t + 1) % T) * N + (s + 1) % N], vertices[t * N + (s + 1) % N],
                    vertices[((t + 1) % T) * N + s]);
            triangles[t * 2 * N + 2 * s] = u;
            triangles[t * 2 * N + 2 * s + 1] = v;
        }
    }

    /* Create neighbour list */
    for (int t = 0; t < T; t++) {
        for (int s = 0; s < N; s++) {
            Triangle::registerNeighbour(triangles[t * 2 * N + 2 * s], triangles[t * 2 * N + 2 * s + 1]);
            Triangle::registerNeighbour(triangles[t * 2 * N + 2 * s + 1], triangles[t * 2 * N + 2 * ((s + 1) % N)]);
            Triangle::registerNeighbour(triangles[t * 2 * N + 2 * s + 1], triangles[((t + 1) % T) * 2 * N + 2 * s]);
        }
    }
     
    /* Check causality to make sure the grid is correct */
	// TODO: remove from production release when satisfied with above algorithm
	for (int t = 0; t < T; t++) {
			for (int s = 0; s < N; s++) {
					if (!vertices[t * N + s]->checkCausality()) {
							std::cerr << "Causality failure at " << t << " " << s << std::endl;
					}	
			}
	} 

    return triangles[0];
}

Triangle * Simulation::Metropolis(Triangle * triangulation) {
    return triangulation;
}

