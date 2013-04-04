#include "observables/SpectralDimensionObservable.h"
#include "observables/Observable.h"
#include "Utils.h"
#include "Vertex.h"
#include "Triangle.h"
#include <boost/math/special_functions/log1p.hpp>
#include <boost/unordered_map.hpp>
#include "Config.h"

SpectralDimensionObservable::SpectralDimensionObservable(unsigned int writeFrequency) :
Observable(writeFrequency, 0, true),
filename(createFilename("specdim")),
file(filename.c_str()),
sigmaMax(READ_CONF("spec.sigmaMax", 1000)),
diffusionConst(READ_CONF("spec.diff", 1.0)),
dualLattice(READ_CONF("spec.dualLattice", false)),
specDim(sigmaMax),
specDim1(sigmaMax) {
}

SpectralDimensionObservable::~SpectralDimensionObservable() {

}

void SpectralDimensionObservable::process(const std::vector<Vertex*>& state) {
    boost::array<std::vector<double>, 2 > probBuffers;
    std::vector< std::vector<unsigned int> > neighbours;

    if (dualLattice) {
        neighbours = buildDualLatticeConnectivity(state);
    } else {
        neighbours = buildLatticeConnectivity(state);
    }

    probBuffers[0] = std::vector<double>(neighbours.size());
    probBuffers[1] = std::vector<double>(neighbours.size());

    unsigned int cur = 0; // current buffer
    unsigned int start = 0; // TODO: average over multiple starting points?
    probBuffers[cur][start] = 1;
    Spec prob(sigmaMax);

    for (unsigned int sigma = 0; sigma < sigmaMax; sigma++) {
        prob[sigma] = probBuffers[cur][start];

        for (int i = 0; i < neighbours.size(); i++) {
            if (probBuffers[cur][i] > epsilon) {
                for (unsigned int n = 0; n < neighbours[i].size(); n++) {
                    // if this node could not be reached in half the number of maximum steps,
                    // it will never reach it back to the starting position
                    if (sigma > sigmaMax / 2 && probBuffers[cur][neighbours[i][n]] < epsilon) {
                        continue;
                    }

                    probBuffers[(cur + 1) % 2][neighbours[i][n]] += diffusionConst *
                            probBuffers[cur][i] / static_cast<double> (neighbours[i].size());
                }
            }

            // there is a probability that the diffusion particles remain where
            // they are. This prevents wild oscillations at low sigma.
            probBuffers[(cur + 1) % 2][i] += (1.0 - diffusionConst) * probBuffers[cur][i];
        }

        // switch buffers
        for (int i = 0; i < neighbours.size(); i++) {
            probBuffers[cur][i] = 0.0;
        }

        cur = (cur + 1) % 2;
    }

    /* Update spectral dimension */
    for (int sigma = 2; sigma < sigmaMax - 1; sigma++) {
        specDim[sigma] = -2.0 * (double) sigma * (prob[sigma + 1] / prob[sigma] - 1.0);
        specDim1[sigma] = -2.0 * log(prob[sigma + 1] / prob[sigma]) /
                boost::math::log1p(1.0 / (double) sigma);
    }
}

void SpectralDimensionObservable::printToScreen() {
}

void SpectralDimensionObservable::printToFile() {
    for (unsigned int sigma = 0; sigma < sigmaMax - 1; sigma++) {
        file << specDim[sigma] << " ";
    }
    file << std::endl;

    for (unsigned int sigma = 0; sigma < sigmaMax - 1; sigma++) {
        file << specDim1[sigma] << " ";
    }
    file << std::endl;
}
