
#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include "2DGraphics.hpp"



SDLWindowManager::SDLWindowManager(unsigned int window_width, unsigned int window_height, std::string title)
{
    if (0 != SDL_Init( SDL_INIT_EVERYTHING ))
    {
        std::cerr << SDL_GetError() << std::endl;
        return;
    }
    if(!SDL_SetVideoMode(window_width, window_height, 32, SDL_OPENGL)) {
        std::cerr << SDL_GetError() << std::endl;
        return;
    }
    SDL_WM_SetCaption("Grid", nullptr);
}

SDLWindowManager::SDLWindowManager(unsigned int window_width, unsigned int window_height) :
SDLWindowManager(window_width, window_height, "SDLWindow")
{}

SDLWindowManager::~SDLWindowManager()
{
    SDL_Quit();
}