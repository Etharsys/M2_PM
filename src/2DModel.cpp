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
    
    GridWindowManager window(500,500,6,6);
    
    bool done = false;
    while(!done)
    {
        SDL_Event e;
        while(SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                done = true;
            }
        }
        window.display_grid<float>( grid, [](float i){ return Color(i,0,i);});
    }
    return 0;
}