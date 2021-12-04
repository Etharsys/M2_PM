#pragma once

/*Structure for positions*/
struct Position
{
    Position() : x(0), y(0) {}
    Position(float _x,float _y) : x(_x), y(_y) {}

    float x;
    float y;
};