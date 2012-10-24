/* 
 * File:   Observable.h
 * Author: bruijl
 *
 * Created on October 22, 2012, 10:59 AM
 */

#ifndef OBSERVABLE_H
#define	OBSERVABLE_H

class Observable {
public:
    Observable() {
        
    }
    
    virtual ~Observable() {
        
    }

    /**
     * Perform a measurement on a certain state.
     * @param state System state
     */
    virtual void measure(const VertSet& state) = 0;

    /**
     * Prints the result of the computation to the screen.
     */
    virtual void printResult() = 0;

    /**
     * Prints the result of the computation to a file.
     */
    virtual void printResult(const char* filename) = 0;
    
    /**
     * Gets the variance of the <b>last</b> n measurements.
     * @param n Should be less than total observations
     */
    virtual double getVariance(int n) = 0;
    
    virtual void getLinearFit(int n, double& a, double& b) = 0;
};

#endif	/* OBSERVABLE_H */

