#include "observables/TimeSliceObservableCDT.h"
#include "Vertex.h"

void TimeSliceObservableCDT::process(const std::vector<Vertex*>& state) {
    Vertex* cur = state[0];
    Vertex* prev = cur->getNeighbouringVertex();
    Triangle *t, *r;

    Vertex::getAdjacentTriangles(cur, prev, &t, &r);
    VertSet sec = cur->getSectorVertices(t, prev, false);
    BOOST_ASSERT(sec.size() == 1);
    prev = *sec.begin();

    for (unsigned int t = 0; t < T; t++) {
        unsigned int count = 0;
        // find loop
        Vertex* lc = cur;
        while (lc != prev) {
            sec = lc->getOtherSectorVertices(cur);
            BOOST_ASSERT(sec.size() == 1); // CDT constraint
            cur = lc;
            lc = *sec.begin();
            count++;
        }

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