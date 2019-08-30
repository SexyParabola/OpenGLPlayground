#pragma once
#include "Triangles.hpp"
#include <vector>

struct rectangle {
    point pos;
    point size;
    color c = color(0, 0, 0);
    rectangle(const point Pos, const point Size, const color C) {
        pos = Pos; size = Size; c = C;
    }
};

struct rectangleManager {
    public:
    std::vector<rectangle> rectangleBuffer;
    rectangleManager() {}
    void addRectangle(const point Pos, const point Size, const color C) {
        rectangleBuffer.pushback(rectangle(Pos, Size, C));
    }
};