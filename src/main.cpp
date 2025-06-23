#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <chrono>
#include <fstream>
#include "loadstl.hpp"
#include <iostream>

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




int main()
{

    float pitch, yaw = 0; // camera vertical, horizontal rotation
    // no roll yet because i dont want to deal with that
    
    float x, y, z = 0;



    const unsigned int WIDTH = 1920u;
    const unsigned int HEIGHT = 1080u;
    const int FPS = 60;

    auto window = sf::RenderWindow(sf::VideoMode({WIDTH, HEIGHT}), "renderer");
    window.setFramerateLimit(FPS);

    log("Renderer v0.1");

    

    std::string stlpath = "C:/Users/ms_lu/Downloads/utahteapot.stl";
    bool ascii = checkASCII(stlpath);


    log("Loading " + stlpath + "...");
    std::vector<std::array<float, 3>>  normals = getNormals(stlpath);
    log("Normals: \t" + std::to_string(normals.size()));
    std::vector<std::array<std::array<float, 3>, 3>> triangles = getTriangles(stlpath);
    log("Triangles: \t" + std::to_string(triangles.size()));
    log("(These two numbers should be the same!)");

    const int SCALE = 10;
    const int offx = 500;
    const int offy = 500;








    if (!font.openFromFile("./Mplus.ttf")) {
        return -1;
    }

    sf::Image frame;
    frame.resize(sf::Vector2u(WIDTH, HEIGHT));
    


    sf::Text text(font);    
    text.setCharacterSize(50);
    text.setFillColor(sf::Color::White);

    debug.setCharacterSize(50);
    debug.setFillColor(sf::Color::Green);
    debug.move(sf::Vector2f(15, 400));

    auto start = std::chrono::high_resolution_clock::now();

    int framecount = 0;

    for(unsigned int i = 50; i < 150; i++) {
        for (unsigned int j = 50; j < 150; j++) {
            
            frame.setPixel(sf::Vector2u(i, j), sf::Color {255, 0, 0, 255});
        }
    }
    

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        framecount++;

        if (framecount > 1 && framecount % FPS == 0) {
            auto end = std::chrono::high_resolution_clock::now();
            
            std::chrono::duration<double> elapsed = end - start;
            start = end;

            double dt = elapsed.count(); 

            double framerate = (1/dt) * FPS;

            text.setString(std::to_string(framerate)); 

        }

        debug.setString(debugmsg);

        window.clear();

        for(int i = 0; i < normals.size(); i++) {
            sf::VertexArray tri(sf::PrimitiveType::Triangles, 3);

            float dp; // simulating viewport vector of (0, 1, 0)
            std::array<float, 3> normal = normals[i];
            if (normal[1] >= 0) {
                continue;
            }



            std::array<std::array<float, 3>, 3> triangle = triangles[i];
            tri[0].position = sf::Vector2f(triangle[0][0] * SCALE + offx, triangle[0][2] * SCALE + offy);
            tri[1].position = sf::Vector2f(triangle[1][0] * SCALE + offx, triangle[1][2] * SCALE + offy);
            tri[2].position = sf::Vector2f(triangle[2][0] * SCALE + offx, triangle[2][2] * SCALE + offy);

            tri[0].color = sf::Color::White;
            tri[1].color = sf::Color::White;
            tri[2].color = sf::Color::White;
            window.draw(tri);
        }
    
        window.draw(text);
        window.draw(debug);
        

        window.display();
        


    }
}