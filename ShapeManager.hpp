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
    std::vector<std::vector<unsigned int>> IDs;
    const std::vector<unsigned int> addTriangle(const point p1, const point p2, const point p3, const color c) {
        std::vector<unsigned int> ID;
        tm.addTriangle(p1, p2, p3, c);
        ID.push_back(tm.triangleBuffer.size() - 1);
        triangleNum++;
        IDs.push_back(ID);
        return ID;
    }
    const std::vector<unsigned int> addRectangle(const point P, const point S, color C) {
        std::vector<unsigned int> ID;
        addTriangle( point(P.x, P.y), point(P.x + S.x, P.y), point(P.x + S.x, P.y + S.y), C);
        ID.push_back(tm.triangleBuffer.size() - 1);
        addTriangle( point(P.x, P.y), point(P.x, P.y + S.y), point(P.x + S.x, P.y + S.y), C);
        ID.push_back(tm.triangleBuffer.size() - 1);
        IDs.push_back(ID);
        return ID;
    };
    void updateColor(std::vector<unsigned int> ID, color C) {
        for (int i = 0; i < ID.size(); i++) {
            tm.triangleBuffer[ID[i]].c = C;
        }
    }
};