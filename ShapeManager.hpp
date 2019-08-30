#pragma once
#include "Triangles.hpp"

class ShapeManager {
public:
    TriagleManager tm;
    unsigned int triangleNum = 0;
    unsigned int squaresNum = 0;
    int addTriangle(const point p1, const point p2, const point p3, const color c) {
        tm.addTriangle(p1, p2, p3, c);
        triangleNum++;
        return triangleNum;
    }
};