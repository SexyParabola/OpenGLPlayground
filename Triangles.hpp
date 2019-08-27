#pragma once
#include <iostream>

struct point {
    float x;
    float y;
    point(float X, float Y) {
        x = X; y = Y;
    } 
    point() {};
};

struct color {
    float r;
    float g;
    float b;
    color(float R, float G, float B) {
        r = R; g = G; b = B;
    }
};

struct triangle {
    point p1;
    point p2;
    point p3;
    color c;
    triangle(const point P1, const point P2, const point P3, const color C) {
        p1 = P1; p2 = P2; p3 = P3; c = C;
    }
};

struct TriagleManager {
    std::vector<triangle> triangleBuffer;
    TriagleManager(){
    }
    const std::vector<float> getVertexBuffer() { 
        std::vector<float> vertexBuffer;
        for (int i = 0; i < triangleBuffer.size(); i++) {
            vertexBuffer.push_back(triangleBuffer[i].p1.x);
            vertexBuffer.push_back(triangleBuffer[i].p1.y);
            vertexBuffer.push_back(triangleBuffer[i].p2.x);
            vertexBuffer.push_back(triangleBuffer[i].p2.y);
            vertexBuffer.push_back(triangleBuffer[i].p3.x);
            vertexBuffer.push_back(triangleBuffer[i].p3.y);
        }
        return vertexBuffer;
    }
    const std::vector<unsigned int> getIndexBuffer() {
        std::vector<unsigned int> indexBuffer;
        for (int i = 0; i < triangleBuffer.size(); i++) { 
            indexBuffer.push_back((i * 3) + 0);
            indexBuffer.push_back((i * 3) + 1);
            indexBuffer.push_back((i * 3) + 2);
        }
        return indexBuffer;
    }
    const std::vector<float> getColorBuffer() {
        std::vector<float> colorBuffer;
        for (int i = 0; i < triangleBuffer.size(); i++) {
            colorBuffer.push_back(triangleBuffer[i].c.r);
            colorBuffer.push_back(triangleBuffer[i].c.g);
            colorBuffer.push_back(triangleBuffer[i].c.b);
        }
    }
    void addTriangle(const point p1, const point const p2, const point p3, const color c){
        triangleBuffer.push_back(triangle(p1, p2, p3, c));
    };
    
};

void printBuffer(std::vector<unsigned int> buffer) {
	std::cout << "[";
	for (int i = 0; i < buffer.size(); i++) {
		if (buffer.size() - 1 != i) {
			std::cout << buffer[i] << ", ";
		}else{
			std::cout << buffer[i] << "]" << std::endl; 
		}
	}
}

void printBuffer(std::vector<float> buffer) {
	std::cout << "[";
	for (int i = 0; i < buffer.size(); i=+2) {
		if (buffer.size() - 1 != i) {
			std::cout << "(" << buffer[i] << "x, " << buffer[i+1] << "y), ";
		}else{
			std::cout << "(" << buffer[i] << "x, " << buffer[i+1] << "y)]" << std::endl; 
		}
	}
}
