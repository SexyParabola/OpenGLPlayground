#pragma once
#include "Triangles.hpp"
#include <iostream>

typedef std::vector<unsigned int> ID;

struct rectangle {
    point pos;
    point size;
    color c = color(0, 0, 0);
    rectangle(const point Pos, const point Size, const color Color) {
        pos = Pos; size = Size; c = Color;
    };
    void print() {
        std::cout << "Pos:(" << pos.x << ", " << pos.y << ")\n";
        std::cout << "Siz:(" << size.x << ", " << size.y << ")\n";
        std::cout << "Color:(R:" << c.r << ", G:" << c.g << ", B:" << c.b << ")\n";
    };
};

class ShapeManager {
public:
    TriangleManager tm;
    unsigned int triangleNum = 0;
    unsigned int rectangleNum = 0;
    //std::vector<ID> IDs;
    const ID addTriangle(const point p1, const point p2, const point p3, const color c) {
        ID id;
        tm.addTriangle(p1, p2, p3, c);; // adds triangle to buffer
        id.push_back(tm.triangleBuffer.size() - 1); // adds added triangle ID to ID vector
        //IDs.push_back(id);
        triangleNum++;
        return id;
    }
    const ID addRectangle(const point P, const point S, color C) {
        ID id;
        addTriangle( point(P.x, P.y), point(P.x + S.x, P.y), point(P.x + S.x, P.y + S.y), C);
        id.push_back(tm.triangleBuffer.size() - 1);
        addTriangle( point(P.x, P.y), point(P.x, P.y + S.y), point(P.x + S.x, P.y + S.y), C);
        id.push_back(tm.triangleBuffer.size() - 1);
        triangleNum += 2;
        //IDs.push_back(ID);
        return id;
    };
    void updateColor(ID id, color C) {
        for (int i = 0; i < id.size(); i++) {
            tm.triangleBuffer[id[i]].c = C;
        }
    }
    void updateRectangle(ID id, rectangle r) {
        tm.triangleBuffer[id[0]] = triangle(
            point(r.pos.x, r.pos.y), 
            point(r.pos.x + r.size.x, r.pos.y),
            point(r.pos.x + r.size.x, r.pos.y + r.size.y), 
            r.c
        );
        tm.triangleBuffer[id[1]] = triangle(
            point(r.pos.x, r.pos.y),
            point(r.pos.x, r.pos.y + r.size.y),
            point(r.pos.x + r.size.x, r.pos.y + r.size.y),
            r.c
        );
    }
    const rectangle getRectangle(ID id) {
        return rectangle(
            point(tm.triangleBuffer[id[0]].p1.x, tm.triangleBuffer[id[0]].p1.y),
            point(tm.triangleBuffer[id[0]].p2.x - tm.triangleBuffer[id[0]].p1.x, tm.triangleBuffer[id[1]].p2.y - tm.triangleBuffer[id[1]].p1.y),
            color(tm.triangleBuffer[id[0]].c)
        );
    }
    // }
    // const rectangle updateRectangle(ID id, const rectangle rect) {
    //     tm.triangleBuffer[id[0]] = triangle(point(rect.pos.x, rect.pos.y), point(rect.pos.x + rect.pos.x, rect.pos.y), point(rect.pos.x + rect.pos.x, rect.pos.y + S.y), rect.c);
    //     addTriangle( point(P.x, P.y), point(P.x, P.y + S.y), point(P.x + S.x, P.y + S.y), C);
    //     //id.push_back(tm.triangleBuffer.size() - 1);
    // }
};

class RectShape {
private:
	ID id;
	ShapeManager* sm;
public:
	RectShape(point Pos, point Size, color Color, ShapeManager& Sm) {
		sm  = &Sm;
		id = sm->addRectangle(Pos, Size, Color);
	}
	RectShape(ShapeManager& Sm) {
		sm  = &Sm;
		id = sm->addRectangle(point(0, 0), point(0, 0), color(0, 0 ,0));
	}
	const point getPos() {
		return sm->getRectangle(id).pos;
	}
	const point getSize() {
		return sm->getRectangle(id).size;
	}
	const color getColor() {
		return sm->getRectangle(id).c;
	}
	void setPos(point Pos) {
		rectangle temp = sm->getRectangle(id);
		temp.pos = Pos;
		sm->updateRectangle(id, temp);
	}
	void setSize(point Size) {
		rectangle temp = sm->getRectangle(id);
		temp.size = Size;
		sm->updateRectangle(id, temp);
	}
	void setColor(color Color) {
		rectangle temp = sm->getRectangle(id);
		temp.c = Color;
		sm->updateRectangle(id, temp);
	}
};