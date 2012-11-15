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
    typedef boost::unordered_map<std::vector<int>, int> freqmap;
    freqmap freq; // state frequency map

public:
    BoltzmannTester() {
        
    }
    
    virtual ~BoltzmannTester() {
        
    }
    
    void addStateId(std::vector<int> stateID) {
        freq[stateID]++;
    }
    
    void printFrequencies() {
        foreach(freqmap::value_type v, freq) {
            std::cout << v.second << " ";
        }
        
        std::cout << std::endl;        
    }
    
};


#endif	/* BOLTMANN_H */

