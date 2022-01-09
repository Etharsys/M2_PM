#pragma once

#include <SDL/SDL.h>

namespace fluid2d
{
    struct Event
    {
        explicit Event(int size);

        Event(int winX, int winY);

        void handleEvent(const SDL_Event &e);

        int win_x, win_y;
        int mouse_down[3]{0, 0, 0};
        int omx{0}, omy{0}, mx{0}, my{0};
    };
}