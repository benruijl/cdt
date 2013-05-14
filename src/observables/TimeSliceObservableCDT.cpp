#include "observables/TimeSliceObservableCDT.h"
#include "Vertex.h"
#include "Triangle.h"

void TimeSliceObservableCDT::process(const std::vector<Vertex*>& state) {
    Vertex* cur = state[0];
    Vertex* future = cur->getNeighbouringVertex();
    Triangle *f, *p;
    Vertex::getAdjacentTriangles(cur, future, &f, &p);
    if (!f->isTimelike(cur, future)) future = f->getThirdVertex(cur, future);

    for (unsigned int t = 0; t < T; t++) {
        unsigned int count = 0;

        // TODO: add TTS assert
        Vertex::getAdjacentTriangles(cur, future, &f, &p);
        Vertex* prev = f->getThirdVertex(cur, future);
        if (f->isTimelike(prev, cur)) prev = p->getThirdVertex(cur, future);

        // find spacelike loop
        Vertex* lc = cur;
        while (lc != prev) {
            VertSet sec = lc->getOtherSectorVertices(cur);
            BOOST_ASSERT(sec.size() == 1); // CDT constraint
            cur = lc;
            lc = *sec.begin();
            count++;
        }

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