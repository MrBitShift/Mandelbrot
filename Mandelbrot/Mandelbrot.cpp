#include <iostream>
#include "Mandelbrot.hpp"
#include "Window.hpp"


int main()
{
    std::cout << "Mandelbrot v0.0.0";

    sf::Window window;
    MakeWindow(window);

    sf::Event windowEvent;
    bool running = true;
    while (running)
    {
        while (window.pollEvent(windowEvent))
        {
            switch (windowEvent.type)
            {
            case sf::Event::Closed:
                running = false;
                break;
            }
        }
    }
}
