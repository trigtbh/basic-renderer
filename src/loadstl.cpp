#include "loadstl.hpp"
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <fstream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

float buffloat(char buffer[4]) {
    float result;
    std::memcpy(&result, buffer, sizeof(float));
    return result;
}


int realtris(char buffer[4]) {
    unsigned char* b = (unsigned char*) buffer;
    float f = 0;
    for (int i = 0; i < 4; i++) {
        f += ((pow(16, 2 * i)) * b[i]);
    }
    return f;
}


std::vector<std::vector<float>> getNormals(std::string filepath) {
    std::ifstream file(filepath, std::ios::binary | std::ios::ate); // open at end
    if (!file) {
        throw std::invalid_argument("invalid file");
    }

    file.seekg(80);
    char buffer[4];
    int tris = 0;
    file.read(buffer, 4);
    tris = realtris(buffer);


    std::vector<std::vector<float>> normals;
    for (int i = 0; i < tris; i++) {
        std::vector<float> normal;
        for (int f = 0; f < 3; f++) {
            file.seekg(84 + 50*i + 3 * f);
            char b[4];
            file.read(b, 4);
            normal.push_back(buffloat(b));
        }
        normals.push_back(normal);

        
    }
    return normals;
}

std::vector<std::vector<std::vector<float>>> getTriangles(std::string filepath) {
    std::ifstream file(filepath, std::ios::binary | std::ios::ate); // open at end
    if (!file) {
        throw std::invalid_argument("invalid file");
    }

    file.seekg(80);
    char buffer[4];
    int tris = 0;
    file.read(buffer, 4);
    tris = buffloat(buffer);
    
    std::vector<std::vector<std::vector<float>>> triangles;
    for (int i = 0; i < tris; i++) {

        std::vector<std::vector<float>> triangle;
        for (int v = 0; v < 3; v++) {
            std::vector<float> vertex;
            for (int f = 0; f < 3; f++) {
                file.seekg(84+50*i + 12 + 3*v + 3*f);
                char b[4];
                file.read(b, 4);
                vertex.push_back(buffloat(b));
            }
            triangle.push_back(vertex);
        }
        triangles.push_back(triangle);
        
    }
    return triangles;
}
