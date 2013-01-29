#include "observables/SpectralDimensionObservable.h"
#include "observables/Observable.h"
#include "Utils.h"
#include "Vertex.h"
#include "Triangle.h"
#include <boost/tuple/tuple.hpp>
#include <boost/unordered_map.hpp>

SpectralDimensionObservable::SpectralDimensionObservable(unsigned int writeFrequency) :
Observable(writeFrequency, 0, true),
filename(createFilename("specdim")),
specDim(sampleSize),
specDim1(sampleSize) {
}

SpectralDimensionObservable::~SpectralDimensionObservable() {

}

/**
 * Because the spectral dimension measurement is slow, a list is built that maps
 * connectivity.
 * @param state
 */
SpectralDimensionObservable::NeighbourList SpectralDimensionObservable::buildConnectivity(const std::vector<Vertex*>& state) {
    boost::unordered_map<Vertex*, unsigned int> index;
    index.rehash(state.size());
    NeighbourList neighbours(state.size());

    for (unsigned int i = 0; i < state.size(); i++) {
        index[state[i]] = i;
    }

    for (unsigned int i = 0; i < state.size(); i++) {

        foreach(Vertex* n, state[i]->getNeighbouringVertices()) {
            neighbours[i].push_back(index[n]);
        }
    }

    return neighbours;
}

void SpectralDimensionObservable::process(const std::vector<Vertex*>& state) {
    boost::array<std::vector<double>, 2 > probBuffers;
    probBuffers[0] = std::vector<double>(state.size());
    probBuffers[1] = std::vector<double>(state.size());

    std::vector< std::vector<unsigned int> > neighbours = buildConnectivity(state);

    unsigned int cur = 0; // current buffer
    unsigned int start = 0; // TODO: average over multiple starting points?
    probBuffers[cur][start] = 1;

    for (unsigned int sigma = 0; sigma < sigmaMax; sigma++) {
        prob[sigma] = probBuffers[cur][start];

        for (int i = 0; i < state.size(); i++) {
            if (probBuffers[cur][i] > epsilon) {
                for (unsigned int n = 0; n < neighbours[i].size(); n++) {
                    // if this node could not be reached in half the number of maximum steps,
                    // it will never reach it back to the starting position
                    if (sigma > sigmaMax / 2 && probBuffers[cur][neighbours[i][n]] < epsilon) {
                        continue;
                    }

                    probBuffers[(cur + 1) % 2][neighbours[i][n]] +=
                            probBuffers[cur][i] / static_cast<double> (neighbours[i].size());
                }
            }
        }

        // switch buffers
        for (int i = 0; i < state.size(); i++) {
            probBuffers[cur][i] = 0.0;
        }

        cur = (cur + 1) % 2;
    }

    /* Update spectral dimension */
    boost::array<double, sigmaMax> spec = {0};
    boost::array<double, sigmaMax> spec1 = {0};
    for (int sigma = 2; sigma < sigmaMax - 1; sigma++) {
        spec[sigma] += -2.0 * (double) sigma * (prob[sigma + 1] / prob[sigma] - 1);
        spec1[sigma] += -2.0 * (log(prob[sigma + 1]) - log(prob[sigma])) /
                (log((double) sigma + 1.0) - log((double) sigma));
    }

    specDim.push_back(spec);
    specDim1.push_back(spec1);
}

void SpectralDimensionObservable::printToScreen() {
}

void SpectralDimensionObservable::printToFile() {
    std::stringstream ss;
    std::string filename_fix = filename;

    if (getMeasurementCount() % 200 == 0) {
        ss << filename << "_" << getMeasurementCount();
        filename_fix = ss.str();
    }
    std::ofstream file(filename_fix.c_str());

    // TODO: make lower bound parameter
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

        /* Calculate standard deviation */
        double std = 0, std1 = 0;

        foreach(Spec& spec, specDim) {
            std += (spec[sigma] - avg) * (spec[sigma] - avg);
        }

        foreach(Spec& spec1, specDim1) {
            std1 += (spec1[sigma] - avg1) * (spec1[sigma] - avg1);
        }

        std = sqrt(std / (double) (specDim.size() - 1));
        std1 = sqrt(std1 / (double) (specDim1.size() - 1));

        file << sigma << " " << avg << " " << std << " " << sigma
                << " " << avg1 << " " << std1 << "\n";
    }
}
