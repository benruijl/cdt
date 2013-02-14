/* 
 * File:   Utils.h
 * Author: bruijl
 *
 * Created on September 27, 2012, 12:08 PM
 */

#ifndef UTILS_H
#define	UTILS_H

#include <vector>
#include <boost/static_assert.hpp>
#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

class Vertex;
class Triangle;

typedef std::vector< std::vector<unsigned int> > NeighbourList;

/**
 * Adds the content of set b to set a.
 * @param a
 * @param b
 * @return 
 */
template <typename T>
        boost::unordered_set<T>& operator +=(boost::unordered_set<T>& a, const boost::unordered_set<T>& b) {

    foreach(T e, b) {
        a.insert(e);
    }

    return a;
}

/**
 * Removes the content of set b from set a.
 * @param a
 * @param b
 * @return 
 */
template <typename T>
        boost::unordered_set<T>& operator -=(boost::unordered_set<T>& a, const boost::unordered_set<T>& b) {

    foreach(T e, b) {
        a.erase(e);
    }

    return a;
}

/**
 * Returns the intersection of two sets.
 * TODO: check for speed
 * @param a Set A
 * @param b Set B
 * @return A & B
 */
template <typename T>
boost::unordered_set<T> operator &(const boost::unordered_set<T>& a, const boost::unordered_set<T>& b) {
    boost::unordered_set<T> out;

    if (b.size() < a.size()) {
        return b & a;
    }

    foreach(T e, a) {
        if (b.find(e) != b.end())
            out.insert(e);
    }

    return out;
}

template <typename T>
unsigned int countOverlap(const boost::unordered_set<T>& a, const boost::unordered_set<T>& b) {
    unsigned int count = 0;

    if (b.size() < a.size()) {
        return countOverlap(b, a);
    }

    foreach(T e, a) {
        if (b.find(e) != b.end())
            count++;
    }

    return count;
}

/**
 * Build a connectivity list of all the vertices. Useful to improve performance
 * of algorithms. The neighbours of the vertices are not ordered and vertex i in
 * state is also called i in the connectivity list.
 * @param state List of all vertices
 * @return 
 */
NeighbourList buildLatticeConnectivity(const std::vector<Vertex*>& state);

NeighbourList buildDualLatticeConnectivity(const std::vector<Vertex*>& state);

boost::unordered_map<Triangle*, unsigned int> createTriangleIds(const std::vector<Vertex*>& state);

NeighbourList buildDualLatticeConnectivity(boost::unordered_map<Triangle*, unsigned int>& index);

#endif	/* UTILS_H */

