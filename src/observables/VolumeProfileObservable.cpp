#include "observables/VolumeProfileObservable.h"
#include "observables/TimeSliceObservable.h"
#include "Simulation.h"
#include <utility>

VolumeProfileObservable::VolumeProfileObservable(unsigned int writeFrequency, TimeSliceObservable* tso,
        Simulation* simulation) :
Observable(writeFrequency, 0, true),
simulation(simulation),
tso(tso),
filename(createFilename("volume")) {

}

VolumeProfileObservable::~VolumeProfileObservable() {

}

void VolumeProfileObservable::process(const std::vector<Vertex*>& state) {
    volumeProfile.clear();

    // FIXME: algorithm now starts at random vertices, not at random time labels
    // the probability that the search will miss the starting point is high, so
    // we also register subloops.
    int N = 100; // sample 100 slices
    for (int i = 0; i < N; i++) {
        Vertex* first = simulation->getRandomVertex(state);
        Vertex* prev = simulation->getRandomElementFromSet(first->getSectorVertices(
                *first->getTriangles().begin(), (*first->getTriangles().begin())->getNextVertex(first), false));

        BOOST_ASSERT(first != prev);
        VertSet visited;
        std::vector<Vertex*> path;
        Vertex* cur = first;
        do {
            if (visited.find(cur) != visited.end()) {
                unsigned int j;
                for (j = 0; j < path.size(); j++) {
                    if (path[j] == cur) {
                        break;
                    }
                }

                path.erase(path.begin(), path.begin() + j);
                //std::cout << "Using subloop of size " << path.size() << " vs. " << visited.size() << std::endl;
                break;
            }

            Vertex* n = simulation->getRandomElementFromSet(cur->getOtherSectorVertices(prev));
            prev = cur;
            cur = n;
            visited.insert(prev);
            path.push_back(prev);
        } while (cur != first);

        double time = 0;

        foreach(Vertex* v, visited) {
            double t = tso->getTimeLabel(v);

            if (t < 0) {
                volumeProfile.clear();
                return; // not every vertex has time label, so vol prof is meaningless
            } else {
                time += tso->getTimeLabel(v);
            }
        }

        volumeProfile.push_back(std::make_pair(time / (double) visited.size(), visited.size()));

    }
}

void VolumeProfileObservable::printToScreen() {

}

void VolumeProfileObservable::printToFile() {
    // TODO: overwrite every time or have persistent storage?
    if (volumeProfile.size() > 0) {
        std::ofstream file(filename.c_str());

        for (unsigned int i = 0; i < volumeProfile.size(); i++) {
            file << volumeProfile[i].first << " " << volumeProfile[i].second << "\n";
        }
    }
}
