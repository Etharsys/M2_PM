#pragma once

/*Structure for positions*/
struct Position
{
    Position() : x(0), y(0), z(0) {}
    Position(float _x,float _y, float _z) : x(_x), y(_y), z(_z) {}

    float x;
    float y;
    float z;
};