#pragma once

#include "Position.hpp"
#include "Color.hpp"

/*@brief used to display the grid*/
class Vertex2D
{
    public:
        Vertex2D(float x, float y, float r, float g, float b) : pos(x,y), col(r,g,b){};
        Vertex2D(float x, float y, Color c) : pos(x,y), col(c){};
        Position pos;
        Color col;
};