#ifndef LOADSTL_HPP
#define LOADSTL_HPP

#include <SFML/Graphics.hpp>


std::vector<std::array<float, 4>> getNormals(std::string filepath);
std::vector<std::array<std::array<float, 4>, 3>> getTriangles(std::string filepath) ;



#endif