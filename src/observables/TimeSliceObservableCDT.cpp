#include "observables/TimeSliceObservableCDT.h"
#include "Vertex.h"
#include "Triangle.h"

void TimeSliceObservableCDT::process(const std::vector<Vertex*>& state) {
    // define a timelike direction that is the future
    Vertex* cur = state[0];
    Vertex* future = cur->getNeighbouringVertex();
    Triangle *f, *p;
    Vertex::getAdjacentTriangles(cur, future, &f, &p);
    if (!f->isTimelike(cur, future)) future = f->getThirdVertex(cur, future);

    for (unsigned int t = 0; t < T; t++) {
        unsigned int count = 0;

        // find spacelike loop at cur
        Vertex::getAdjacentTriangles(cur, future, &f, &p);
        VertSet n = cur->getSectorVertices(f, future, false);
        BOOST_ASSERT(n.size() == 1); // CDT constraint

        Vertex* prev = *n.begin();
        Vertex* lc = cur;
        do {
            VertSet sec = lc->getOtherSectorVertices(prev);
            BOOST_ASSERT(sec.size() == 1); // CDT constraint
            prev = lc;
            lc = *sec.begin();
            count++;
        } while (lc != cur);

        Vertex* newfut = *future->getOtherSectorVertices(cur).begin();
        cur = future;
        future = newfut;

        volumeProfile[t] = count;
    }
}

TimeSliceObservableCDT::TimeSliceObservableCDT(unsigned int writeFrequency, unsigned int T) :
Observable(writeFrequency, 1, false),
filename(createFilename("cdtslice")),
file(filename.c_str()),
T(T),
volumeProfile(T) {

}

TimeSliceObservableCDT::~TimeSliceObservableCDT() {

}

void TimeSliceObservableCDT::printToScreen() {

}

void TimeSliceObservableCDT::printToFile() {
    for (unsigned int t = 0; t < T; t++) {
        file << volumeProfile[t] << " ";
    }

    file << std::endl;
}