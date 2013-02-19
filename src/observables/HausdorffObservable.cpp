#include <queue>

#include "observables/HausdorffObservable.h"
#include "Utils.h"

void HausdorffObservable::process(const std::vector<Vertex*>& state) {
    NeighbourList neighbours = buildDualLatticeConnectivity(state);

    // TODO: calculate average over all starting positions
    int cur = 0, steps = 0;
    unsigned int content = 1;
    std::vector<char> visited(neighbours.size());
    std::queue<unsigned int> queue;
    queue.push(cur);
    visited[cur] = true;

    area.resize(state.size());

    while (!queue.empty()) {
        cur = queue.front();
        queue.pop();

        content = 0;
        for (int i = 0; i < neighbours[cur].size(); i++) {
            if (!visited[neighbours[cur][i]]) {
                content++;
                queue.push(neighbours[cur][i]);
                visited[neighbours[cur][i]] = true;
            }
        }

        area[steps] = content;
        steps++;
    }
}

HausdorffObservable::HausdorffObservable(unsigned long writeFrequency) :
Observable(writeFrequency, 0, true),
filename(createFilename("haus")) {

}

HausdorffObservable::~HausdorffObservable() {

}

void HausdorffObservable::printToScreen() {

}

void HausdorffObservable::printToFile() {
    file.open(filename.c_str());

    for (int i = 0; i < area.size(); i++) {
        file << i << " " << area[i] << std::endl;
    }

}
