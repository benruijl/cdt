#include <queue>
#include <utility>

#include "observables/HausdorffObservable.h"
#include "Utils.h"
#include "Simulation.h"

std::vector<unsigned int> HausdorffObservable::getDistribution(NeighbourList& neighbours, unsigned int start) {
    int cur = start, steps = 0, shellcount = 0, newshell = 1;
    std::vector<char> visited(neighbours.size());
    std::queue<unsigned int> queue;
    queue.push(cur);
    visited[cur] = true;

    std::vector<unsigned int> area(neighbours.size());

    while (!queue.empty()) {
        /* Are we going to a new shell? */
        if (shellcount == 0) {
            shellcount = newshell;
            area[steps] = shellcount;
            newshell = 0;
            steps++;
        }

        cur = queue.front();
        queue.pop();

        for (int i = 0; i < neighbours[cur].size(); i++) {
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

double HausdorffObservable::getExtent(std::vector<unsigned int>& area, unsigned int norm) {
    double ext = 0;

    for (unsigned int i = 0; i < area.size(); i++) {
        ext += i * area[i];
    }

    // TODO: divide by area size (= number of steps) or volume?
    ext /= (double) norm;

    return ext;
}

void HausdorffObservable::process(const std::vector<Vertex*>& state) {
    NeighbourList neighbours = buildDualLatticeConnectivity(state);
    boost::uniform_int<> uint(0, neighbours.size() - 1);

    dist.clear();
    dist.resize(sqrt(neighbours.size())*1.5); // FIXME, what should this be?
    
    for (int i = 0; i < numSamples; i++) {
        std::vector<unsigned int> area = getDistribution(neighbours, uint(simulation->getRNG()));

        BOOST_ASSERT(area.size() < sqrt(neighbours.size())*1.5);
        
        /* Average area*/
        for (int k = 0; k < area.size(); k++) {
            dist[k] += area[k];
        }
    }
    
    for (int i = 0; i < dist.size(); i++) {
        dist[i] /= (double) numSamples;
    }
}

HausdorffObservable::HausdorffObservable(Simulation* simulation,
        unsigned long writeFrequency) :
Observable(writeFrequency, 0, true),
filename(createFilename("haus")),
file(filename.c_str()),
numSamples(READ_CONF("samples", 100)),
simulation(simulation) {
}

HausdorffObservable::~HausdorffObservable() {

}

void HausdorffObservable::printToScreen() {

}

void HausdorffObservable::printToFile() {
    for (int i = 0; i < dist.size(); i++) {
        file << dist[i] << " ";
    }

    file << std::endl;
}
