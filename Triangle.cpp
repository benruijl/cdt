/* 
 * File:   Triangle.cpp
 * Author: ben
 * 
 * Created on September 19, 2012, 3:31 PM
 */

#include "Triangle.h"

Triangle::Triangle(TYPE type) {
    this->type = type;
}

Triangle::Triangle(const Triangle& orig) {
    this->type = orig.type;
    vertices[0] = orig.vertices[0];
    vertices[1] = orig.vertices[1];
    vertices[2] = orig.vertices[2];
}

Triangle::~Triangle() {
}

