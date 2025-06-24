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

float x = 0;
float y = 0;
float z = 0;

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

const unsigned int WIDTH = 1920u;
const unsigned int HEIGHT = 1080u;
const int FPS = 120;





std::array<float, 4> project(
    std::array<float, 4> point 
) {
    std::array<float, 4> ret;
    ret = matmul(TRANSFORM, point); // Model

    
    
    point[0] -= cx;
    point[1] -= cy;
    point[2] -= cz;

    ret = matmul(rotateZ(-yaw), ret);
    ret = matmul(rotateX(-pitch), ret);



    return ret;

}



void drawAxes(sf::RenderWindow& window, float axisLength = 100.0f) {
    // Create axis lines - X (red), Y (green), Z (blue)
    std::vector<std::array<std::array<float, 4>, 2>> axisLines = {
        // X-axis (red) - from origin to positive X
        {{
            {{0, 0, 0, 1}},  // origin
            {{axisLength, 0, 0, 1}}   // X direction
        }},
        // Y-axis (green) - from origin to positive Y  
        {{
            {{0, 0, 0, 1}},  // origin
            {{0, axisLength, 0, 1}}   // Y direction
        }},
        // Z-axis (blue) - from origin to positive Z
        {{
            {{0, 0, 0, 1}},  // origin
            {{0, 0, -axisLength, 1}}   // Z direction
        }}
    };
    
    sf::Color axisColors[3] = {sf::Color::Red, sf::Color::Green, sf::Color::Blue};
    
    for(int axis = 0; axis < 3; axis++) {
        sf::VertexArray line(sf::PrimitiveType::Lines, 2);
        
        // Project both points of the line
        std::array<float, 4> start = project(axisLines[axis][0]);
        std::array<float, 4> end = project(axisLines[axis][1]);
        
        // Convert to screen coordinates - using your current mapping
        line[0].position = sf::Vector2f(start[0] + WIDTH / 2, start[2] + HEIGHT / 2);
        line[1].position = sf::Vector2f(end[0] + WIDTH / 2, end[2] + HEIGHT / 2);
        
        line[0].color = axisColors[axis];
        line[1].color = axisColors[axis];
        
        window.draw(line);
    }
}





int main()
{


    
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

    bool frameskip = false;

    
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            if (frameskip) {
                frameskip = false;
            } else if (window.hasFocus()) {

                if (const auto* wheel = event->getIf<sf::Event::MouseWheelScrolled>()) {
                    if (wheel->delta > 0) {
                        update = true;
                        scale += 5;
                    } else {
                        update = true;
                        scale -= 5;
                    }
                    SCALE_MATRIX = {{
                        {{scale, 0, 0, 0}},
                        {{0, scale, 0, 0}},
                        {{0, 0, scale, 0}},
                        {{0, 0, 0, 1}}
                    }};
                }


            if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
                // Fix: Use window center instead of desktop center
                sf::Vector2i windowCenter = sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2);
                int dx = mouseMoved->position.x - windowCenter.x;
                int dy = mouseMoved->position.y - windowCenter.y;

                if (dx != 0 || dy != 0) {
                    update = true;
                    
                    float sensitivity = 0.00025f;
                    yaw += dx * sensitivity;
                    pitch += dy * sensitivity;  
                    
                    
                    const float maxPitch = 1.57f; 
                    if (pitch > maxPitch) {
                        pitch = maxPitch;
                    }
                    if (pitch < -maxPitch) {
                        pitch = -maxPitch;
                    }
                    while (yaw > 6.28318f) { 
                        yaw -= 6.28318f;
                    }
                    while (yaw < 0) {
                        yaw += 6.28318f;
                    }

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

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
                cx += 500.0f / FPS;
                update = true;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
                cx -= 500.0f / FPS;
                update = true;
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

            int drawn = 0;
            
            for(int i = 0; i < normals.size(); i++) {
                

                std::array<float, 4> normal = normals[i];

                normal = matmul(ROTATION_MATRIX_Z, normal);
                normal = matmul(rotateZ(-yaw), normal);
                normal = matmul(rotateX(-pitch), normal);

                if (normal[1] < 0) {
                    continue;
                }
                
                drawn++;

                std::array<std::array<float, 4>, 3> triangle = triangles[i];

                std::array<std::array<float, 4>, 3> t1 = {{
                    project(triangle[0]),
                    project(triangle[1]),
                    project(triangle[2])
                }};

                //std::array<std::array<float, 4>, 3> t1 = triangle;


                tri[0].position = sf::Vector2f(t1[0][0] + WIDTH / 2, t1[0][2] + HEIGHT / 2);
                tri[1].position = sf::Vector2f(t1[1][0] + WIDTH / 2, t1[1][2] + HEIGHT / 2);
                tri[2].position = sf::Vector2f(t1[2][0] + WIDTH / 2, t1[2][2] + HEIGHT / 2);

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

            drawAxes(window, 100.0f);
            

            std::cout << "Drawn: " << drawn << "\n";
            window.display();    
            
            update = false;      
        }


        
        


    }
}