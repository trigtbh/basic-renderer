#ifndef LOADSTL_HPP
#define LOADSTL_HPP

#include <SFML/Graphics.hpp>


std::vector<std::vector<float>> getNormals(std::string filepath);
std::vector<std::vector<std::vector<float>>> getTriangles(std::string filepath);



#endif