#include <SFML/Graphics.hpp>
//#include <SFML/Window/Keyboard.hpp>
#include <stdlib.h>
#include <chrono>
#include <fstream>
#include "loadstl.hpp"
#include <iostream>
#include "project.hpp"

sf::Font font;
sf::Text debug(font);

std::string debugmsg;

void log(std::string t) {
    std::cout << t << "\n";
}

bool checkASCII(std::string filepath) {
    const std::string filename = filepath;
    const std::size_t bytesToRead = 80;

    std::ifstream file(filename, std::ios::binary | std::ios::ate); // open at end
    if (!file) {
        throw std::invalid_argument("invalid file");
    }

    std::streamsize fileSize = file.tellg(); // get size
    if (fileSize < bytesToRead) {
        // log("2");
        throw std::invalid_argument("invalid file");
    }

    file.seekg(0); // go back to beginning
    std::vector<char> buffer(bytesToRead);
    file.read(buffer.data(), bytesToRead);

    std::string s;

    for (char c : buffer) {
        s += c;
    }

    if (s.find("solid ") != std::string::npos) {
        return true;
    }
    // log("3");
    return false;
}


float rotZ = 0;
std::array<std::array<float, 4>, 4> ROTATION_MATRIX_Z = rotateZ(rotZ);

float x = 500;
float y = 0;
float z = 500;

std::array<std::array<float, 4>, 4> TRANSLATION = {{
    {{1, 0, 0, x}},
    {{0, 1, 0, y}},
    {{0, 0, 1, z}},
    {{0, 0, 0, 1}}
}};

float scale = 10;

std::array<std::array<float, 4>, 4> SCALE_MATRIX = {{
    {{scale, 0, 0, 0}},
    {{0, scale, 0, 0}},
    {{0, 0, scale, 0}},
    {{0, 0, 0, 1}}
}};


std::array<std::array<float, 4>, 4> TRANSFORM = TRANSFORM = consolidate(TRANSLATION, consolidate(ROTATION_MATRIX_Z, SCALE_MATRIX));

float cx, cy, cz = 0;
float pitch, yaw = 0.5; 

std::array<std::array<float, 4>, 4> CAMERA_POSITION = {{
    {{1, 0, 0, cx}},
    {{0, 1, 0, cy}},
    {{0, 0, 1, cz}},
    {{0, 0, 0, 1}}
}};






std::array<float, 4> project(
    std::array<float, 4> point 
) {
    std::array<float, 4> ret;
    ret = matmul(TRANSFORM, point); // Model

    return ret;

}




int main()
{


    const unsigned int WIDTH = 1920u;
    const unsigned int HEIGHT = 1080u;
    const int FPS = 120;

    auto window = sf::RenderWindow(sf::VideoMode({WIDTH, HEIGHT}), "renderer");

    // window.setMouseCursorGrabbed(true);
    window.setFramerateLimit(FPS);

    // window.setMouseCursorVisible(false);

    
    log("Renderer v0.1");


    sf::Vector2i windowCenter = window.getPosition() + sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2);
    sf::Mouse::setPosition(windowCenter);
    window.setMouseCursorVisible(false);

    std::string stlpath = "C:/Users/ms_lu/Downloads/utahteapot.stl";
    bool ascii = checkASCII(stlpath);


    log("Loading " + stlpath + "...");
    std::vector<std::array<float, 4>>  normals = getNormals(stlpath);
    log("Normals: \t" + std::to_string(normals.size()));
    std::vector<std::array<std::array<float, 4>, 3>> triangles = getTriangles(stlpath);
    log("Triangles: \t" + std::to_string(triangles.size()));
    log("(These two numbers should be the same!)");


    if (!font.openFromFile("./Mplus.ttf")) {
        return -1;
    }


    auto start = std::chrono::high_resolution_clock::now();
    int framecount = 0;
    bool update = true;

    bool mousecapture = true;


    
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            if (window.hasFocus()) {
            if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
                    int dx = mouseMoved->position.x - (sf::VideoMode::getDesktopMode().size.x / 2);
                    int dy = mouseMoved->position.y - (sf::VideoMode::getDesktopMode().size.y / 2);

                    
                    if (dx != 0 || dy != 0) {
                        //std::cout << dx << ", " << dy << "\n";
                        // rotation sensitivity = 500
                        pitch -= dy / 1000.0f; // z-correction ?
                        yaw += dx / 1000.0f;
                        if (pitch < 0) {
                            pitch = 0;
                        } else if (pitch > 1) {
                            pitch = 1;
                        }

                        if (yaw > 1) {
                            yaw -= 1;
                        } 
                        if (yaw < -1) {
                            yaw += 1;
                        } 


                        std::cout << pitch << ", " << yaw << "\n";
                        if (mousecapture) {
                            sf::Mouse::setPosition(windowCenter, window);
                        }  
                    }
                }
            }

            if (event->is<sf::Event::FocusLost>()) {
                window.setMouseCursorVisible(true);
                mousecapture = false;
            }

            if (event->is<sf::Event::FocusGained>()) {
                window.setMouseCursorVisible(false);
                mousecapture = true;
            }

            
            

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
                rotZ += (2.0f / FPS);
                if (rotZ >= 1) { rotZ -= 2; }

                update = true;
                ROTATION_MATRIX_Z = rotateZ(2.0f * 3.14159265f * rotZ);


            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
                rotZ -= (2.0f / FPS);
                if (rotZ < 0) { rotZ += 1 ;}
                
                update = true;
                ROTATION_MATRIX_Z = rotateZ(2.0f * 3.14159265f * rotZ);
            }
        }

        framecount++;

        if (framecount % FPS == 0) {
            auto end = std::chrono::high_resolution_clock::now();
            double framerate = FPS / std::chrono::duration<double>(end - start).count();
            
            start = end;
        }
         
        if (update) {
            TRANSFORM = consolidate(TRANSLATION, consolidate(ROTATION_MATRIX_Z, SCALE_MATRIX));  
            window.clear();
            sf::VertexArray tri(sf::PrimitiveType::Triangles, 3);

            for(int i = 0; i < normals.size(); i++) {
                

                float dp; // simulating viewport vector of (0, 1, 0)
                std::array<float, 4> normal = normals[i];

                normal = matmul(ROTATION_MATRIX_Z, normal);


                if (normal[1] >= 0) {
                    continue;
                }


                std::array<std::array<float, 4>, 3> triangle = triangles[i];

                std::array<std::array<float, 4>, 3> t1 = {{
                    project(triangle[0]),
                    project(triangle[1]),
                    project(triangle[2])
                }};

                //std::array<std::array<float, 4>, 3> t1 = triangle;


                tri[0].position = sf::Vector2f(t1[0][0], t1[0][2]);
                tri[1].position = sf::Vector2f(t1[1][0], t1[1][2]);
                tri[2].position = sf::Vector2f(t1[2][0], t1[2][2]);

                if (triangles[i][0][0] > 0) {
                    tri[0].color = sf::Color::Red;
                    tri[1].color = sf::Color::Red;
                    tri[2].color = sf::Color::Red;
                } else {
                    tri[0].color = sf::Color::White;
                    tri[1].color = sf::Color::White;
                    tri[2].color = sf::Color::White;
                }
                
                window.draw(tri);
                
            }

            // system("pause");
        
            window.draw(debug);
            

            window.display();    
            
            update = false;      
        }


        
        


    }
}