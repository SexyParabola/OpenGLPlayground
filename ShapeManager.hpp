#pragma once
#include "Triangles.hpp"

class rectangle {
    point pos;
    point size;
    color c = color(0, 0, 0);
    rectangle(const point Pos, const point Size, const color Color) {
        pos = Pos; size = Size; c = Color;
    };
};

class ShapeManager {
public:
    TriangleManager tm;
    unsigned int triangleNum = 0;
    unsigned int rectangleNum = 0;
    int addTriangle(const point p1, const point p2, const point p3, const color c) {
        tm.addTriangle(p1, p2, p3, c);
        triangleNum++;
        return triangleNum;
    }
    int addRectangle(const point P, const point S, color C) {
        addTriangle( point(P.x, P.y), point(P.x + S.x, P.y), point(P.x + S.x, P.y + S.y), C);
        addTriangle( point(P.x, P.y), point(P.x, P.y + S.y), point(P.x + S.x, P.y + S.y), C);
        rectangleNum++;
        return rectangleNum;
    };
};