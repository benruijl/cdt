#include "moves/Move.h"

Move::Move(int Ntts, int Ntss) : fixedTriangle(NULL), Ntss(Ntss), Ntts(Ntts) {
}

Move::~Move() {

}

double Move::getMoveProbability(double lambda, double alpha) {
    // note that alpha should be positive
    return exp(-lambda / 4.0 * (sqrt(4.0 * alpha - 1.0) * (double)Ntts +
            sqrt(alpha * (4.0 - alpha)) * (double)Ntss));
}
