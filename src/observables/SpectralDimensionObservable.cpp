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
specDim(sampleSize),
specDim1(sampleSize),
sigmaMax(READ_CONF("spec.sigmaMax", 1000)),
diffusionConst(READ_CONF("spec.diff", 1.0)),
sampleSize(READ_CONF("spec.sampleSize", 7000)),
dualLattice(READ_CONF("spec.dualLattice", false)) {
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
    Spec spec(sigmaMax);
    Spec spec1(sigmaMax);
    for (int sigma = 2; sigma < sigmaMax - 1; sigma++) {
        spec[sigma] += -2.0 * (double) sigma * (prob[sigma + 1] / prob[sigma] - 1.0);
        spec1[sigma] += -2.0 * log(prob[sigma + 1] / prob[sigma]) /
                boost::math::log1p(1.0 / (double) sigma);
    }

    specDim.push_back(spec);
    specDim1.push_back(spec1);
}

void SpectralDimensionObservable::printToScreen() {
}

void SpectralDimensionObservable::printToFile() {
    std::ofstream file(filename.c_str());

    // TODO: make lower bound a parameter
    for (unsigned int sigma = 10; sigma < sigmaMax - 1; sigma++) {
        double avg = 0, avg1 = 0;

        foreach(Spec& spec, specDim) {
            avg += spec[sigma];
        }

        foreach(Spec& spec1, specDim1) {
            avg1 += spec1[sigma];
        }

        avg /= (double) specDim.size();
        avg1 /= (double) specDim1.size();

        /* Calculate standard error */
        double std = 0, std1 = 0;

        foreach(Spec& spec, specDim) {
            std += (spec[sigma] - avg) * (spec[sigma] - avg);
        }

        foreach(Spec& spec1, specDim1) {
            std1 += (spec1[sigma] - avg1) * (spec1[sigma] - avg1);
        }

        std = sqrt(std / ((double) (specDim.size() - 1) * (double) specDim.size()));
        std1 = sqrt(std1 / ((double) (specDim1.size() - 1) * (double) specDim1.size()));

        file << sigma << " " << avg << " " << std << " " << sigma
                << " " << avg1 << " " << std1 << "\n";
    }
}
