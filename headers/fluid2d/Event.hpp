#pragma once

#include <SDL/SDL.h>
#include <tuple>

namespace fluid2d
{
    struct Event
    {
        Event(int size, float force);

        Event(int winX, int winY, float force);

        void handleEvent(const SDL_Event &e);

        // Setters

        void reset_movement();

        // Getters

        bool left_click() const;

        bool middle_click() const;

        bool right_click() const;

        int winX() const;

        int winY() const;

        float x_offset() const;

        float y_offset() const;

        std::tuple<int, int> get_position(float N) const;

    private:
        int win_x, win_y;
        float force;
        bool left{false}, middle{false}, right{false};
        int omx{0}, omy{0}, mx{0}, my{0};
    };
}