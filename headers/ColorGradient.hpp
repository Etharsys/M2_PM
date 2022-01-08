#pragma once

#include <Color.hpp>
#include <initializer_list>
#include <vector>

class ColorGradient
{
public:
    ColorGradient(std::initializer_list<Color> colors...);

    Color get_color(float t) const;

private:
    unsigned int get_index(float t) const;

    std::vector<Color> colors;
    unsigned int n;
};
