#include "observables/SpectralDimensionObservable.h"
#include "observables/Observable.h"
#include "Utils.h"
#include "Vertex.h"
#include <boost/tuple/tuple.hpp>
#include <boost/unordered_map.hpp>

SpectralDimensionObservable::SpectralDimensionObservable(unsigned int writeFrequency) :
Observable(writeFrequency, 0, true),
filename(createFilename("specdim")),
specDim(sampleSize) {
}

SpectralDimensionObservable::~SpectralDimensionObservable() {

}

void SpectralDimensionObservable::process(const std::vector<Vertex*>& state) {
    boost::array<boost::unordered_map<Vertex*, double>, 2 > probBuffers;
    unsigned int cur = 0;

    Vertex* start = state[0];
    probBuffers[cur][start] = 1;

    for (unsigned int sigma = 0; sigma < sigmaMax; sigma++) {

        prob[sigma] = probBuffers[cur][start];

        Vertex* key;
        double value;

        foreach(boost::tie(key, value), probBuffers[cur]) {

            foreach(Vertex* n, key->getNeighbouringVertices()) {
                // if this node could not be reached in half the number of maximum steps,
                // it will never reach it back to the starting position
                if (sigma > sigmaMax / 2 && probBuffers[cur].find(n) == probBuffers[cur].end()) {
                    continue;
                }

                probBuffers[(cur + 1) % 2][n] += 1.0 / (double) key->getNeighbouringVertexCount() *
                        value;
            }
        }

        // switch buffers
        probBuffers[cur].clear();
        cur = (cur + 1) % 2;
    }

    /* Update spectral dimension */
    boost::array<double, sigmaMax> spec = {0};
    for (int sigma = 1; sigma < sigmaMax - 1; sigma++) {
        spec[sigma] += -2.0 * (double) sigma * (prob[sigma + 1] / prob[sigma] - 1);
    }

    specDim.push_back(spec);
}

void SpectralDimensionObservable::printToScreen() {
}

void SpectralDimensionObservable::printToFile() {
    std::ofstream file(filename.c_str());

    // TODO: make lower bound parameter
    for (unsigned int sigma = 10; sigma < sigmaMax - 1; sigma++) {
        double avg = 0;

        foreach(Spec& spec, specDim) {
            avg += spec[sigma];
        }
        avg /= (double) specDim.size();

        file << sigma << " " << avg << "\n";
    }
}
