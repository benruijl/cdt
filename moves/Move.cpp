#include "Move.h"

Move::Move(int Ntts, int Ntss) : fixedTriangle(NULL) {
    this->Ntss = Ntss;
    this->Ntts = Ntts;
}

Move::~Move() {

}

double Move::getMoveProbability(double lambda, double alpha) {
    return exp(-lambda / 4.0 * (sqrt(1 - 4 * alpha) * Ntts +
            sqrt(-alpha * (4 - alpha)) * Ntss));
}
