/* 
 * File:   Boltmann.h
 * Author: bruijl
 *
 * Created on November 14, 2012, 3:05 PM
 */

#ifndef BOLTMANN_H
#define	BOLTMANN_H

#include <boost/unordered_map.hpp>

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

        // TODO: retrieving action this way only works for the CDT case
        // FIXME: does not take into account degeneracies!
        double invz = exp(-lambda / 4.0 * (sqrt(1 - 4 * alpha))) - 1;

        foreach(freqmap::value_type v, freq) {
            double action = exp(-lambda / 4.0 * (sqrt(1 - 4 * alpha) * 
            ((v.first.size() -2) / 2.0))); // TODO: check the num tri is correct
            double count = action * invz * total;

            std::cout << (int)round(count - v.second) << " ";
        }

        std::cout << std::endl;
    }

};


#endif	/* BOLTMANN_H */

