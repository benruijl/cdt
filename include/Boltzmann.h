/* 
 * File:   Boltmann.h
 * Author: bruijl
 *
 * Created on November 14, 2012, 3:05 PM
 */

#ifndef BOLTMANN_H
#define	BOLTMANN_H

#include <boost/unordered_map.hpp>
#include <fstream>

/**
 * This class keeps track of how many times the Monte Carlo process ends up in a
 * given state. Given enough time, the probabilities should be equal to the 
 * probabilities of the action, due to detailed balance.
 */
class BoltzmannTester {
private:
    typedef boost::unordered_map<std::vector<int>, unsigned long long> freqmap;
    freqmap freq; // state frequency map
    unsigned long long total;
public:

    BoltzmannTester() : total(0) {

    }

    virtual ~BoltzmannTester() {

    }

    void addStateId(std::vector<int> stateID) {
        freq[stateID]++;
        total++;
    }

    void printFrequencies(double lambda, double alpha) {
        /* Calculate the difference between the real frequencies and expected
         frequencies based on lambda and alpha.*/
        std::cout << "There are " << freq.size() << " states" << std::endl;

        std::ofstream file("freq.dat");

        /* Print data points */
        foreach(freqmap::value_type v, freq) {
            file << (v.first.size() - 2) / 2.0 << " " <<
                    v.second << " " << v.second / (double) total << "\n";
        }
    }

};


#endif	/* BOLTMANN_H */

