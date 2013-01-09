#include "observables/VolumeProfileObservable.h"
#include "observables/TimeSliceObservable.h"
#include "Simulation.h"
#include <utility>

VolumeProfileObservable::VolumeProfileObservable(unsigned int writeFrequency, TimeSliceObservable* tso,
        Simulation* simulation) :
tso(tso),
simulation(simulation),
Observable(writeFrequency, 0, true),
filename(createFilename("volume")) {

}

VolumeProfileObservable::~VolumeProfileObservable() {

}

void VolumeProfileObservable::process(const std::vector<Vertex*>& state) {
    volumeProfile.clear();

    // FIXME: algorithm now starts at random vertices, not at random time labels
    int N = 10; // sample 100 slices
    for (int i = 0; i < N; i++) {
        Vertex* first = simulation->getRandomVertex(state);
        Vertex* prev = simulation->getRandomElementFromSet(first->getSectorVertices(
                *first->getTriangles().begin(), (*first->getTriangles().begin())->getNextVertex(first), false));

        VertSet visited;
        Vertex* cur = first;
        do {
            if (visited.find(cur) != visited.end()) {
                break; // fail, subloop
            }

            Vertex* n = simulation->getRandomElementFromSet(cur->getOtherSectorVertices(prev));
            prev = cur;
            cur = n;
            visited.insert(cur);
        } while (cur != first);

        if (cur == first) {
            double time = 0;

            foreach(Vertex* v, visited) {
                double t = tso->getTimeLabel(v);

                if (t > 0) {
                    time += tso->getTimeLabel(v);
                } else {
                    break; // note every vertex has 
                }
            }

            volumeProfile.push_back(std::make_pair(time / (double) visited.size(), visited.size()));
        }
    }
}

void VolumeProfileObservable::printToScreen() {

}

void VolumeProfileObservable::printToFile() {
    // TODO: overwrite every time or have persistent storage?
    std::ofstream file(filename.c_str());

    for (int i = 0; i < volumeProfile.size(); i++) {
        file << volumeProfile[i].first << " " << volumeProfile[i].second << "\n";
    }
}
