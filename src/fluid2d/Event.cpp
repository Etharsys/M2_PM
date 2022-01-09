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
            left |= e.button.button == SDL_BUTTON_LEFT;
            right |= e.button.button == SDL_BUTTON_RIGHT;
        }
        else if (e.type == SDL_MOUSEBUTTONUP)
        {
            if (e.button.button == SDL_BUTTON_LEFT)
            {
                left = false;
            }
            if (e.button.button == SDL_BUTTON_RIGHT)
            {
                right = false;
            }
        }
        else if (e.type == SDL_MOUSEMOTION)
        {
            mx = e.motion.x;
            my = e.motion.y;
        }
    }

    // Setters

    void Event::reset_movement()
    {
        omx = mx;
        omy = mx;
    }

    // Getters

    bool Event::left_click() const
    {
        return left;
    }

    bool Event::middle_click() const
    {
        return middle;
    }

    bool Event::right_click() const
    {
        return right;
    }

    int Event::winX() const
    {
        return win_x;
    }

    int Event::winY() const
    {
        return win_y;
    }

    float Event::x_offset() const
    {
        return static_cast<float>(mx - omx);
    }

    float Event::y_offset() const
    {
        return static_cast<float>(my - omy);
    }

    std::tuple<int, int> Event::get_position(float N) const
    {
        auto i = 1 + (static_cast<float>(mx) / static_cast<float>(win_x)) * N;
        auto j = 1 + (static_cast<float>(my) / static_cast<float>(win_y)) * N;
        return {i, j};
    }
}
