#include "Window.hpp"

void MakeWindow(sf::Window& window)
{
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 2;
	// Opengl 4.6
	settings.majorVersion = 4;
	settings.minorVersion = 6;
	settings.attributeFlags = sf::ContextSettings::Core;

	window.create(sf::VideoMode(800, 600), "Mandelbrot", sf::Style::Close, settings);

	window.setFramerateLimit(60);
	window.setActive();
}