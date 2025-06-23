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

    



    const unsigned int WIDTH = 1920u;
    const unsigned int HEIGHT = 1080u;
    const int FPS = 120;

    auto window = sf::RenderWindow(sf::VideoMode({WIDTH, HEIGHT}), "renderer");
    window.setFramerateLimit(FPS);

    log("Renderer v0.1");

    

    std::string stlpath = "C:/Users/ms_lu/Downloads/utahteapot.stl";
    bool ascii = checkASCII(stlpath);


    log("Loading " + stlpath + "...");
    std::vector<std::vector<float>> normals = getNormals(stlpath);
    log("Normals: \t" + std::to_string(normals.size()));
    std::vector<std::vector<std::vector<float>>> triangles = getTriangles(stlpath);
    log("Triangles: \t" + std::to_string(triangles.size()));
    log("(These two numbers should be the same!)");



    if (!font.openFromFile("Mplus.ttf")) {
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
    
    // log("ok!");

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

        sf::Texture texture;
        if(!texture.loadFromImage(frame)) {
            return -1;
        }
        sf::Sprite sprite(texture);
        window.draw(sprite);
    
        window.draw(text);
        window.draw(debug);
        

        window.display();
        


    }
}