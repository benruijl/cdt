#include "moves/FlipMove.h"
#include "Triangle.h"
#include <deque>
#include "Utils.h"

FlipMove::FlipMove(bool timelike, bool change) :
Move(-2 * change * (timelike * 2 - 1), 2 * change * (timelike * 2 - 1)) {
    isTimelike = timelike;
    this->change = change;
}

double FlipMove::getTransitionProbability(std::vector<Vertex*>& vertices) {
    return 1.0 / (vertices.size() * u->getNeighbouringVertexCount()) +
            1.0 / (vertices.size() * v->getNeighbouringVertexCount());
}

bool FlipMove::isMovePossible(std::vector<Vertex*>& vertices) {
    Triangle* first, *second;
    Vertex::getAdjacentTriangles(u, v, &first, &second);
    Vertex* c = first->getThirdVertex(u, v);
    Vertex* d = second->getThirdVertex(u, v);

    /* A fixed triangle should not be deleted. */
    if (first == getFixedTriangle() || second == getFixedTriangle()) {
        return false;
    }

    // prevent link overlap, could happen in a very rare cases
    if (c->isNeighbour(d)) {
        return false;
    }

    return first->isTimelike(u, v) == isTimelike &&
            first->isTimelike(u, c) != second->isTimelike(u, d)
            && first->isTimelike(v, c) != second->isTimelike(v, d);
}

Move* FlipMove::generateRandomMove(Simulation& simulation) {
    u = simulation.getRandomVertex(simulation.getVertices());
    v = simulation.getRandomElementFromSet(u->getNeighbouringVertices());
    return this;
}

double FlipMove::getInverseTransitionProbability(std::vector<Vertex*>& vertices) {
    Triangle* first, *second;
    Vertex::getAdjacentTriangles(u, v, &first, &second);

    Vertex* c = first->getThirdVertex(u, v);
    Vertex* d = second->getThirdVertex(u, v);

    return 1.0 / (vertices.size() * (c->getNeighbouringVertexCount() + 1)) +
            1.0 / (vertices.size() * (d->getNeighbouringVertexCount() + 1));
}

// call for bubble removal move
// link should be timelike
// TODO: should also work for spacelike?
// a and b should be two vertices on the outside of the the rhombus!
// e and f are extra vertices

bool FlipMove::hasSelfOverlappingBubbles2(Vertex* a, Vertex* b, Vertex* e, Vertex* f) {
    typedef std::pair<Vertex*, Vertex*> curPrev;
    VertSet visited;
    std::deque<curPrev> queue;
    queue.push_back(curPrev(a, b));
    queue.push_back(curPrev(b, a)); // required?

    while (!queue.empty()) {
        curPrev c = queue.back();
        queue.pop_back();

        visited.insert(c.first);

        Vertex* prev = c.second;
        if (c.first == b && c.second == f) prev = e;
        if (c.first == f && c.second == b) prev = a;
        VertSet o = c.first->getSameSectorVertices(prev);
        if (c.first == b && o.find(a) != o.end()) o.insert(f);
        if (c.first == f && o.find(e) != o.end()) o.insert(b);

        foreach(Vertex* n, o) {
            if (n != c.second) { // this could skip vertices like (f,e)
                Vertex* prev = c.first; // change prev so other sector vertices works
                if (n == b && c.first == f) prev = a;
                if (n == f && c.first == b) prev = e;


                // check if the hit is due to compactification or overlap
                if (visited.find(n) != visited.end()) {

                    foreach(Vertex* k, n->getOtherSectorVertices(prev)) {
                        if (visited.find(k) != visited.end()) {
                            return true; // found overlap
                        }
                    }

                    // TODO: what to do here? this means compactification is found,
                    // is it also ok to just stop?
                    continue;
                }

                queue.push_back(curPrev(n, c.first)); // depth first
            }
        }
    }

    return false;
}

void FlipMove::execute(std::vector<Vertex*>& vertices) {
    Triangle* first, *second;
    Vertex::getAdjacentTriangles(u, v, &first, &second);

    if (isTimelike && hasSelfOverlappingBubbles(u, v)) {
        std::cout << "SELF OVERLAPPING BUBBLES FLIP CREATED ELSEWHERE" << std::endl;
        std::cout << u << " " << v << std::endl;
        
        foreach(Vertex* k, vertices) {
            k->printConnectivity();
        }
        
        BOOST_ASSERT(false);
    } else std::cout << isTimelike << " " << change << std::endl;

    Vertex* c = first->getThirdVertex(u, v);
    Vertex* d = second->getThirdVertex(u, v);

    /* Get link types */
    bool lNew = first->isTimelike(u, v) ^ change;
    bool lAC = first->isTimelike(u, c);
    bool lCB = first->isTimelike(c, v);
    bool lAD = second->isTimelike(u, d);
    bool lBD = second->isTimelike(v, d);

    /*  if (change && !isTimelike) { // bubble merge move     
          if (lAC) {
              if (this->hasSelfOverlappingBubbles2(u, c, v, d)) {
                  std::cout << "SELF OVERLAPPING BUBBLE DETECTED, BAILING OUT" << std::endl;
                  return;
              }
          } else
              if (this->hasSelfOverlappingBubbles2(u, d, v, c)) {
              std::cout << "SELF OVERLAPPING BUBBLE DETECTED, BAILING OUT" << std::endl;
              return;
          }
      }*/

    Triangle* t1 = new Triangle(u, c, d, lAC, lNew, lAD);
    Triangle* t2 = new Triangle(c, v, d, lCB, lBD, lNew);

    first->removeFromVertices();
    second->removeFromVertices();
    delete first;
    delete second;

    BOOST_ASSERT(u->checkCausality());
    BOOST_ASSERT(v->checkCausality());
    BOOST_ASSERT(c->checkCausality());
    BOOST_ASSERT(d->checkCausality());

    if (!isTimelike && change) {
        if (hasSelfOverlappingBubbles(c, d)) {
            std::cout << "SELF OVERLAPPING BUBBLES CHANGE FLIP, REVERTING!" << std::endl;

            // revert
            new Triangle(u, c, v, lAC, lCB, isTimelike);
            new Triangle(u, d, v, lAD, lBD, isTimelike);

            t1->removeFromVertices();
            t2->removeFromVertices();
            delete t1;
            delete t2;
            
            BOOST_ASSERT(hasSelfOverlappingBubbles(u, v) == false);

            return;
        }
    }

    // if spacelike
    /* if ((!isTimelike && !change) || (isTimelike && change)) {
         if ((t1->isTimelike(u, c) && hasSelfOverlappingBubbles(u, c))
                 || (t1->isTimelike(u, d) && hasSelfOverlappingBubbles(u, d))) {
             std::cout << "SELF OVERLAPPING BUBBLES CHANGE FLIP, REVERTING!" << std::endl;
             u = c;
             v = d;
             isTimelike = (isTimelike && !change) || (!isTimelike && change);
             docheck = false;
             execute(vertices);
         }
         if ((t2->isTimelike(v, c) && hasSelfOverlappingBubbles(v, c))
                 || (t2->isTimelike(v, d) && hasSelfOverlappingBubbles(v, d))) {
             std::cout << "SELF OVERLAPPING BUBBLES CHANGE FLIP, REVERTING!" << std::endl;
             u = c;
             v = d;
             isTimelike = (isTimelike && !change) || (!isTimelike && change);
             docheck = false;
             execute(vertices);

         }
     }

     if (((isTimelike && !change) || (!isTimelike && change)) && hasSelfOverlappingBubbles(c, d)) {
            std::cout << "SELF OVERLAPPING BUBBLES FLIP, REVERTING!" << std::endl;
            u = c;
            v = d;
            isTimelike = (isTimelike && !change) || (!isTimelike && change);
            execute(vertices);
        } else 
            std::cout << "NO OVERLAP!" << std::endl;*/
}

std::string FlipMove::printID() {
    return std::string("FLIP_") + (isTimelike ? "TL" : "SL") + (change ? "_CH" : "");
}
