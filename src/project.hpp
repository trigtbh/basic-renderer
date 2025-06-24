#ifndef PROJECT_HPP
#define PROJECT_HPP

#include <array>

std::array<float, 2> project(
    const std::array<float, 3>& cameraPos,
    const std::array<float, 3>& cameraVec,
    const std::array<float, 3>& point
);

std::array<float, 3> cross(std::array<float, 3> a, std::array<float, 3> b);
float dot(std::array<float, 3> a, std::array<float, 3> b);


std::array<float, 4> normalize(
    const std::array<float, 4>& vec
);

std::array<float, 3> normalize(
    const std::array<float, 3>& vec
);

std::array<float, 4> matmul(
    const std::array<std::array<float, 4>, 4>& a,
    const std::array<float, 4>& b
);


std::array<std::array<float, 4>, 4> rotateX(
    float rx
);

std::array<std::array<float, 4>, 4> rotateY(
    float ry
);


std::array<std::array<float, 4>, 4> rotateZ(
    float rz
);

std::array<std::array<float, 4>, 4> consolidate(
    const std::array<std::array<float, 4>, 4>& a,
    const std::array<std::array<float, 4>, 4>& b
);

#endif