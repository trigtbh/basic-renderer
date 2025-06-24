#include "project.hpp"
#include <fstream>
#include <stdlib.h>
#include <cmath>





std::array<float, 4> matmul(
    const std::array<std::array<float, 4>, 4>& a,
    const std::array<float, 4>& b
) {

    std::array<float, 4> ret;
    ret[0] = b[0] * a[0][0] + b[1] * a[0][1] + b[2] * a[0][2] + b[3] * a[0][3];
    ret[1] = b[0] * a[1][0] + b[1] * a[1][1] + b[2] * a[1][2] + b[3] * a[1][3];
    ret[2] = b[0] * a[2][0] + b[1] * a[2][1] + b[2] * a[2][2] + b[3] * a[2][3];
    ret[3] = b[0] * a[3][0] + b[1] * a[3][1] + b[2] * a[3][2] + b[3] * a[3][3];

    return ret;

}


std::array<std::array<float, 4>, 4> rotateX(
    float rx
) {
    using namespace std;

    float cx = cos(rx);
    float sx = sin(rx);

    std::array<std::array<float, 4>, 4> ret = {{
        {{1, 0, 0, 0}}, 
        {{0, cx, -sx, 0}}, 
        {{0, sx, cx, 0}}, 
        {{0, 0, 0, 1}}
    }};
    return ret;
};

std::array<std::array<float, 4>, 4> rotateY(
    float ry
) {

    using namespace std;

    float cy = cos(ry);
    float sy = sin(ry);


    std::array<std::array<float, 4>, 4> ret =  {{
        {{cy, 0, sy, 0}}, 
        {{0, 1, 0, 0}}, 
        {{-sy, 0, cy, 0}}, 
        {{0, 0, 0, 1}}
    }};
    return ret;
};


std::array<std::array<float, 4>, 4> rotateZ(
    float rz
) {
    using namespace std;

    float cz = cos(rz);
    float sz = sin(rz);

    std::array<std::array<float, 4>, 4> ret =  {{
        {{cz, -sz, 0, 0}}, 
        {{sz, cz, 0, 0}}, 
        {{0, 0, 1, 0}}, 
        {{0, 0, 0, 1}}
    }};
    
    return ret;
};


std::array<std::array<float, 4>, 4> consolidate(
    const std::array<std::array<float, 4>, 4>& a,
    const std::array<std::array<float, 4>, 4>& b
) {

    std::array<std::array<float, 4>, 4> ret;
    for (int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {

            ret[i][j] = a[i][0] * b[0][j] + a[i][1] * b[1][j] + a[i][2] * b[2][j] + a[i][3] * b[3][j];



        }
    }

    return ret;



}