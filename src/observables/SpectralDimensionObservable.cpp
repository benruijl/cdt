#include "observables/SpectralDimensionObservable.h"
#include "observables/Observable.h"
#include "Utils.h"
#include "Vertex.h"
#include <boost/tuple/tuple.hpp>

SpectralDimensionObservable::SpectralDimensionObservable(unsigned int writeFrequency) :
Observable(writeFrequency, 0, true),
filename(createFilename("specdim")),
file(filename.c_str()) {
}

SpectralDimensionObservable::~SpectralDimensionObservable() {

}

void SpectralDimensionObservable::process(const std::vector<Vertex*>& state) {
    specDim.clear();

    // TODO: make vector?
    boost::unordered_map<Vertex*, double> nextProb, curProb;

    Vertex* start = state[0];
    curProb[start] = 1;
    boost::unordered_map<Vertex*, double>* curBuffer = &curProb;


    unsigned int sigmaMax = 50; // TODO: make parameter
    for (unsigned int sigma = 0; sigma < sigmaMax; sigma++) {
        specDim[sigma] = curProb[start];

        Vertex* key;
        double value;

        foreach(boost::tie(key, value), *curBuffer) {

            foreach(Vertex* n, key->getNeighbouringVertices()) {
                nextProb[n] += 1.0 / (double) key->getNeighbouringVertexCount() *
                        value; // fixme: should not be nextprob but !curBuffer
            }
        }

        // switch buffers
        curBuffer->clear();
        curBuffer = curBuffer == &curProb ? &nextProb : &curProb;
    }
}

void SpectralDimensionObservable::printToScreen() {
}

void SpectralDimensionObservable::printToFile() {
    file.seekp(0);

    unsigned int key;
    double value;

    foreach(boost::tie(key, value), specDim) {
        file << key << " " << value << "\n";
    }
}
