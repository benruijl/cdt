/* 
 * File:   AcceptanceRateObservable.h
 * Author: bruijl
 *
 * Created on April 25, 2013, 11:39 AM
 */

#ifndef ACCEPTANCERATEOBSERVABLE_H
#define	ACCEPTANCERATEOBSERVABLE_H

#include "Observable.h"

class AcceptanceRateObservable : public Observable {
private:
    MoveFactory* moveFactory;
    std::string filename;
    std::ofstream file;

    void process(const std::vector<Vertex*>& state) {
        MoveFactory::MoveStatistics moveStatistics = moveFactory->getMoveStatistics();
        const char* names[] = {"AL_TL", "AL_SL", "FL_C", "FL", "CO_TL", "CO_SL", "P_SL",
            "P_TL"};

        // FIXME: move
        for (unsigned int i = 0; i < moveStatistics.size(); i++) {
            file << names[i % 8];

            if (i >= 8) {
                file << "_I";
            }

            file << " ";

            for (unsigned int j = 0; j < moveStatistics[i].size(); j++) {
                file << moveStatistics[i][j] << " ";
            }

            file << std::endl;
        }
        file << std::endl;
    }
public:

    AcceptanceRateObservable(unsigned long writeFrequency, MoveFactory* moveFactory) :
    Observable(writeFrequency, 1, false),
    moveFactory(moveFactory),
    filename(createFilename("acc")),
    file(filename.c_str()) {
    }

    virtual ~AcceptanceRateObservable() {

    }

    void printToScreen() {
    }

    /**
     * Prints the result of the computation to a file.
     */
    void printToFile() {
    }
};

#endif	/* ACCEPTANCERATEOBSERVABLE_H */

