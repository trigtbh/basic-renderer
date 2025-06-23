#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <chrono>

struct Pixel {
    int r, g, b;
    Pixel(int r, int g, int b) {

    };

    Pixel() {
        r = 0;
        g = 0;
        b = 0;
    };
};

int main()
{

    const unsigned int WIDTH = 1920u;
    const unsigned int HEIGHT = 1080u;
    const int FPS = 120;

    std::vector<std::vector<Pixel>> screen(HEIGHT, std::vector<Pixel>(WIDTH));

    auto window = sf::RenderWindow(sf::VideoMode({WIDTH, HEIGHT}), "CMake SFML Project");
    window.setFramerateLimit(FPS);

    sf::Font font;
    if (!font.openFromFile("Mplus.ttf")) {
        return -1;
    }


    sf::Text text(font);
    
    text.setCharacterSize(50);
    text.setFillColor(sf::Color::White);


    auto start = std::chrono::high_resolution_clock::now();

    int framecount = 0;
    

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