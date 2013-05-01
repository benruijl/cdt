#include <queue>
#include <utility>

#include "observables/HausdorffObservable.h"
#include "Utils.h"
#include "Simulation.h"

std::vector<unsigned int> HausdorffObservable::getDistribution(const NeighbourList& neighbours,
        unsigned int start) {
    unsigned int cur = start, steps = 0, shellcount = 0, newshell = 1;
    std::vector<char> visited(neighbours.size());
    std::queue<unsigned int> queue;
    queue.push(cur);
    visited[cur] = true;

    std::vector<unsigned int> area(neighbours.size());

    while (!queue.empty()) {
        /* Are we going to a new shell? */
        if (shellcount == 0) {
            shellcount = newshell;
            BOOST_ASSERT(steps < neighbours.size());
            area[steps] = shellcount;
            newshell = 0;
            steps++;
        }

        cur = queue.front();
        queue.pop();
        BOOST_ASSERT(cur < neighbours.size());

        for (unsigned int i = 0; i < neighbours[cur].size(); i++) {
            BOOST_ASSERT(neighbours[cur][i] < neighbours.size());
            if (!visited[neighbours[cur][i]]) {
                newshell++;
                queue.push(neighbours[cur][i]);
                visited[neighbours[cur][i]] = true;
            }
        }

        shellcount--;
    }

    area.resize(steps);

    return area;
}

void HausdorffObservable::process(const std::vector<Vertex*>& state) {
    NeighbourList neighbours = buildDualLatticeConnectivity(state);
    std::vector< std::vector<unsigned int> > areas(numSamples);
    unsigned int step = neighbours.size() / numSamples;
    if (step == 0) step = 1;

#pragma omp parallel for
    for (unsigned int i = 0; i < numSamples; i++) {
        areas[i] = getDistribution(neighbours, i * step);
    }

    dist.clear();

    for (unsigned int j = 0; j < numSamples; j++) {
        if (dist.size() < areas[j].size()) {
            dist.resize(areas[j].size());
        }
        
        for (unsigned int i = 0; i < areas[j].size(); i++) {
            dist[i] += areas[j][i];
        }
    }

    for (unsigned int i = 0; i < dist.size(); i++) {
        dist[i] /= (double) numSamples;
    }
}

HausdorffObservable::HausdorffObservable(Simulation* simulation,
        unsigned long writeFrequency) :
Observable(writeFrequency, 0, true),
filename(createFilename("haus")),
file(filename.c_str()),
numSamples(READ_CONF("haus.samples", 1000)),
simulation(simulation) {
}

HausdorffObservable::~HausdorffObservable() {

}

void HausdorffObservable::printToScreen() {

}

void HausdorffObservable::printToFile() {
    for (unsigned int i = 0; i < dist.size(); i++) {
        file << dist[i] << " ";
    }

    file << std::endl;
}
