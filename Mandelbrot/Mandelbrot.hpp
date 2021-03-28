#ifndef MANDELBROT_HPP
#define MANDELBROT_HPP

#define GLEW_STATIC
#define SFML_STATIC
#define SFML_SYSTEM_WINDOWS

#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/System/Time.hpp>

extern "C" __declspec(dllexport) unsigned long NvOptimusEnablement = 1;
extern "C" __declspec(dllexport) unsigned long AmdPowerXpressRequestHighPerformance = 1;

#include <chrono>
#include <iostream>
#include <thread>
#include <fstream>
#include <streambuf>

#endif // MANDELBROT_HPP