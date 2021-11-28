#include "2DGraphics.hpp"
#include <vector>
#include <iostream>
#include <iterator>

int main()
{
    std::vector<float> grid = 
    {.1,0.,0.,0.,0.,1.,
     0.,.2,0.,0.,0.,0.,
     0.,0.,.3,0.,0.,0.,
     0.,1.,0.,.4,0.,0.,
     0.,0.,0.,0.,0.5,0.,
     0.,0.,0.,0.,0.,0.6};
    
    SDLWindowManager window(500,500);
    
    window.display_grid<float>(6, 6, grid, [](float i){ return Color(i,i,i);});
    return 0;
}