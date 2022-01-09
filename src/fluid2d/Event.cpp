#include <fluid2d/Event.hpp>

namespace fluid2d
{
    fluid2d::Event::Event(int size) :
            win_x(size),
            win_y(size)
    {
    }

    fluid2d::Event::Event(int winX, int winY) :
            win_x(winX),
            win_y(winY)
    {
    }

    void fluid2d::Event::handleEvent(const SDL_Event &e)
    {
        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            omx = mx = e.button.x;
            omy = my = e.button.y;
            if (e.button.button == SDL_BUTTON_LEFT)
            {
                mouse_down[0] = 1;
            }
            if (e.button.button == SDL_BUTTON_RIGHT)
            {
                mouse_down[2] = 1;
            }
        }
        else if (e.type == SDL_MOUSEBUTTONUP)
        {
            if (e.button.button == SDL_BUTTON_LEFT)
            {
                mouse_down[0] = 0;
            }
            if (e.button.button == SDL_BUTTON_RIGHT)
            {
                mouse_down[2] = 0;
            }
        }
        else if (e.type == SDL_MOUSEMOTION)
        {
            mx = e.motion.x;
            my = e.motion.y;
        }
    }
}
