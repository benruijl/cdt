/* 
 * File:   Boltmann.h
 * Author: bruijl
 *
 * Created on November 14, 2012, 3:05 PM
 */

#ifndef BOLTMANN_H
#define	BOLTMANN_H

#include <boost/unordered_map.hpp>
#include <boost/date_time.hpp>
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
    unsigned long long total; // should be large enough
    std::string filename;
public:

    BoltzmannTester() : total(0) {
        boost::posix_time::time_facet* facet = new boost::posix_time::time_facet("%d%m%Y_%H%M%S");
        boost::posix_time::ptime cur(boost::posix_time::second_clock::local_time());
        std::stringstream ss;
        ss.imbue(std::locale(ss.getloc(), facet));
        ss << "freq_" << cur << ".dat";
        filename = ss.str();
    }

    virtual ~BoltzmannTester() {

    }

    void addStateId(std::vector<int> stateID) {
        freq[stateID]++;
        total++;
        
        if (total > -3) {
            std::cerr << "WARNING: total states out of range";
        }
    }

    void printFrequencies(double lambda, double alpha) {
        /* Calculate the difference between the real frequencies and expected
         frequencies based on lambda and alpha.*/
        std::cout << "There are " << freq.size() << " states" << std::endl;

        std::ofstream file(filename.c_str());

        /* Print data points */
        foreach(freqmap::value_type v, freq) {
            file << (v.first.size() - 2) / 2.0 << " " <<
                    v.second << " " << v.second / (double) total << "\n";
        }
    }

};


#endif	/* BOLTMANN_H */

