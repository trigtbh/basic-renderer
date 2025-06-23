#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <chrono>

int main()
{

    const unsigned int WIDTH = 1920u;
    const unsigned int HEIGHT = 1080u;
    const int FPS = 120;

    auto window = sf::RenderWindow(sf::VideoMode({WIDTH, HEIGHT}), "CMake SFML Project");
    window.setFramerateLimit(FPS);

    sf::Font font;
    if (!font.openFromFile("Mplus.ttf")) {
        return -1;
    }

    sf::Image frame;
    frame.resize(sf::Vector2u(WIDTH, HEIGHT));
    


    sf::Text text(font);    
    text.setCharacterSize(50);
    text.setFillColor(sf::Color::White);


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

        window.clear();

        sf::Texture texture;
        texture.loadFromImage(frame);
        sf::Sprite sprite(texture);
        window.draw(sprite);
    
        window.draw(text);
        

        window.display();
        


    }
}

#ifdef _WIN32
#include <windows.h>

// Forward declare your main
extern int main();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    return main();
}
#endif