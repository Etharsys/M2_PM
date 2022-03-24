#pragma once

#include "Position.hpp"
#include "Color.hpp"

/*@brief used to display the grid*/
class Vertex3D
{
    public:
        Vertex3D(float x, float y, float z, float r, float g, float b) : pos(x,y,z), col(r,g,b){};
        Vertex3D(float x, float y, float z, Color c) : pos(x,y,z), col(c){};
        Position pos;
        Color col;
};