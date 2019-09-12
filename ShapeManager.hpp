#pragma once
#include "Triangles.hpp"

typedef std::vector<unsigned int> ID;

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
    std::vector<ID> IDs;
    const ID addTriangle(const point p1, const point p2, const point p3, const color c) {
        ID id; // 
        tm.addTriangle(p1, p2, p3, c);;
        id.push_back(tm.triangleBuffer.size() - 1);
        IDs.push_back(id);
        triangleNum++;
        return id;
    }
    const ID addRectangle(const point P, const point S, color C) {
        ID ID;
        addTriangle( point(P.x, P.y), point(P.x + S.x, P.y), point(P.x + S.x, P.y + S.y), C);
        ID.push_back(tm.triangleBuffer.size() - 1);
        addTriangle( point(P.x, P.y), point(P.x, P.y + S.y), point(P.x + S.x, P.y + S.y), C);
        ID.push_back(tm.triangleBuffer.size() - 1);
        IDs.push_back(ID);
        return ID;
    };
    void updateColor(ID ID, color C) {
        for (int i = 0; i < ID.size(); i++) {
            tm.triangleBuffer[ID[i]].c = C;
        }
    }
};