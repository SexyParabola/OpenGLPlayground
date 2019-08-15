#pragma once

struct point {
    float x;
    float y;
    point(float X, float Y) {
        x = X; y = Y;
    } 
    point() {};
};

struct triangle {
    point p1;
    point p2;
    point p3;
    triangle(const point P1, const point P2, const point P3) {
        p1 = P1; p2 = P2; p3 = P3;
    }
};

struct TriagleManager {
    std::vector<triangle> triangleBuffer;
    TriagleManager(){
    }
    const std::vector<point> getVertexBuffer() { 
        std::vector<point> vertexBuffer;
        for (int i = 0; i < triangleBuffer.size(); i++) {
            vertexBuffer.push_back(triangleBuffer[i].p1);
            vertexBuffer.push_back(triangleBuffer[i].p2);
            vertexBuffer.push_back(triangleBuffer[i].p3);
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
    void addTriangle( point p1, point p2, point p3){
        triangleBuffer.push_back(triangle(p1, p2, p3));
    };
    
};
