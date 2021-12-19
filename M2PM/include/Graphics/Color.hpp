#pragma once

/*Structure for colors*/
struct Color
{
    Color() : r(0), g(0), b(0){}
    Color(float _r, float _g, float _b) : r(_r), g(_g), b(_b){}

    float r;
    float g;
    float b;
};