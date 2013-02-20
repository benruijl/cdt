#include <queue>
#include <utility>

#include "observables/HausdorffObservable.h"
#include "Utils.h"

void HausdorffObservable::process(const std::vector<Vertex*>& state) {
    NeighbourList neighbours = buildDualLatticeConnectivity(state);

    // TODO: calculate average over all starting positions
    int cur = 0, steps = 0, shellcount = 0, newshell = 1;
    std::vector<char> visited(neighbours.size());
    std::queue<unsigned int> queue;
    queue.push(cur);
    visited[cur] = true;

    area.resize(neighbours.size());

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

    double ext = 0;

    for (int i = 0; i < steps; i++) {
        ext += i * area[i];
    }

    ext /= (double) neighbours.size();

    extent.push_back(std::make_pair(neighbours.size(), ext));
}

HausdorffObservable::HausdorffObservable(unsigned long writeFrequency) :
Observable(writeFrequency, 0, true),
filename(createFilename("haus")),
file(filename.c_str()) {

}

HausdorffObservable::~HausdorffObservable() {

}

void HausdorffObservable::printToScreen() {

}

void HausdorffObservable::printToFile() {
    file << extent.back().first << " " << extent.back().second << std::endl;
}
