#include "2DGraphics.hpp"
#include <vector>
#include <iostream>
#include <iterator>

int main()
{
    std::vector<float> grid = 
    {1.,0.,0.,0.,0.,1.,
     0.,1.,0.,0.,0.,0.,
     0.,0.,1.,0.,0.,0.,
     0.,0.,0.,1.,0.,0.,
     0.,0.,0.,0.,1.,0.,
     0.,0.,0.,0.,0.,1.};
    
    SDLWindowManager window(500,500);
    
    window.display_grid<float>(6, 6, grid, [](float i){ return i;});
    return 0;
}