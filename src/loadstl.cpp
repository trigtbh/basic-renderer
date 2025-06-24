#include "loadstl.hpp"
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <fstream>
#include <math.h>
#include <stdio.h>
#include <iostream>

float buffloat(char buffer[4]) {
    float result;
    std::memcpy(&result, buffer, sizeof(float));
    std::cout << result << "\n";
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


std::vector<std::array<float, 4>> getNormals(std::string filepath) {
    std::ifstream file(filepath, std::ios::binary | std::ios::ate); // open at end
    if (!file) {
        throw std::invalid_argument("invalid file");
    }

    file.seekg(80);
    int tris;
    file.read(reinterpret_cast<char*>(&tris), sizeof(int));



    std::vector<std::array<float, 4>> normals;
    for (int i = 0; i < tris; i++) {
        std::array<float, 4> normal;
        for (int f = 0; f < 3; f++) {
            file.seekg(84 + 50*i + 4 * f);
            float fl;
            file.read(reinterpret_cast<char*>(&fl), sizeof(float));
            
            if (f == 2) { fl *= -1; } // z correction
            
            normal[f] = fl;
        }
        normal[3] = 0; // w = 0 for normals
        normals.push_back(normal);

        
    }
    return normals;
}

std::vector<std::array<std::array<float, 4>, 3>> getTriangles(std::string filepath) {
    std::ifstream file(filepath, std::ios::binary | std::ios::ate); // open at end
    if (!file) {
        throw std::invalid_argument("invalid file");
    }

    file.seekg(80);
    int tris;
    file.read(reinterpret_cast<char*>(&tris), sizeof(int));
    
    std::vector<std::array<std::array<float, 4>, 3>> triangles;
    for (int i = 0; i < tris; i++) {

        std::array<std::array<float, 4>, 3> triangle;
        for (int v = 0; v < 3; v++) {
            std::array<float, 4> vertex;
            
            for (int f = 0; f < 3; f++) {
                file.seekg(84+50*i + 12 + 12*v + 4*f);
                float fl;
                file.read(reinterpret_cast<char*>(&fl), sizeof(float));
                if (f == 2) { fl *= -1; } // z correction

                vertex[f] = fl;
            }
            vertex[3] = 1; // w = 1 for points
            triangle[v] = vertex;
        }
        triangles.push_back(triangle);
        
    }
    return triangles;
}
