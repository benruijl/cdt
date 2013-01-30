#include "observables/SpectralDimensionObservable.h"
#include "observables/Observable.h"
#include "Utils.h"
#include "Vertex.h"
#include "Triangle.h"
#include <boost/math/special_functions/log1p.hpp>
#include <boost/unordered_map.hpp>

SpectralDimensionObservable::SpectralDimensionObservable(unsigned int writeFrequency) :
Observable(writeFrequency, 0, true),
filename(createFilename("specdim")),
specDim(sampleSize),
specDim1(sampleSize){
}

SpectralDimensionObservable::~SpectralDimensionObservable() {

}

SpectralDimensionObservable::NeighbourList
SpectralDimensionObservable::buildDualLatticeConnectivity(const std::vector<Vertex*>& state) {
    boost::unordered_map<Triangle*, unsigned int> index;
    index.rehash(state.size() * 2);
    NeighbourList neighbours(state.size() * 2);

    int id = 0;
    for (unsigned int i = 0; i < state.size(); i++) {

        foreach(Triangle* t, state[i]->getTriangles()) {
            if (index.find(t) == index.end()) {
                index[t] = id;
                id++;
            }
        }
    }

    std::pair<Triangle*, unsigned int> p;

    foreach(p, index) {
        for (int i = 0; i < 3; i++) {
            neighbours[p.second].push_back(index[p.first->getNeighbour(i)]);
        }
    }

    return neighbours;
}

SpectralDimensionObservable::NeighbourList SpectralDimensionObservable::
buildLatticeConnectivity(const std::vector<Vertex*>& state) {
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
    std::vector< std::vector<unsigned int> > neighbours = buildDualLatticeConnectivity(state);
    probBuffers[0] = std::vector<double>(neighbours.size());
    probBuffers[1] = std::vector<double>(neighbours.size());

    unsigned int cur = 0; // current buffer
    unsigned int start = 0; // TODO: average over multiple starting points?
    probBuffers[cur][start] = 1;

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

                    probBuffers[(cur + 1) % 2][neighbours[i][n]] +=
                            probBuffers[cur][i] / static_cast<double> (neighbours[i].size());
                }
            }
        }

        // switch buffers
        for (int i = 0; i < neighbours.size(); i++) {
            probBuffers[cur][i] = 0.0;
        }

        cur = (cur + 1) % 2;
    }

    /* Update spectral dimension */
    boost::array<double, sigmaMax> spec = {0};
    boost::array<double, sigmaMax> spec1 = {0};
    for (int sigma = 2; sigma < sigmaMax - 1; sigma++) {
        spec[sigma] += -2.0 * (double) sigma * (prob[sigma + 1] / prob[sigma] - 1);
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
