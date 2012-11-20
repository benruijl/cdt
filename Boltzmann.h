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
            // TODO: retrieving action this way only works for the CDT 
            //double action = exp(-lambda / 4.0 * (sqrt(1 - 4 * alpha) * 
            //((v.first.size() -2) / 2.0)));
            file << (v.first.size() - 2) / 2.0 << " " << v.second / (double) total << "\n";
        } 
    }

};


#endif	/* BOLTMANN_H */

