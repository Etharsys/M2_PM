#include "Graphics/2DGraphics.hpp"
#include <vector>
#include <iostream>
#include <iterator>

int main()
{
    std::vector<float> grid(100000000);
    /*
    {.1,0.,0.,0.,0.,0.,0.,0.,0.,
     0.,.2,0.,0.,0.,0.,0.,0.,0.,
     0.,0.,.3,0.,0.,0.,0.,0.,0.,
     0.,1.,0.,.4,0.,0.,0.,0.,0.,
     0.,0.,0.,0.,0.5,0.,0.,0.,0.,
     0.,0.,0.,0.,0.,0.6,0.,0.,0.,
     0.,0.,0.,0.,0.,0.,0.,0.,0.,
     0.,0.,0.,0.,0.,0.,0.,0.,0.,
     0.,0.,0.,0.,0.,0.,0.,0.,0.,
     0.,0.,0.,0.,0.,0.,0.,0.,0.,};
     */
    for (int i = 0; i < 9999; i++){
        for (int j = 0; j < 50; j++){
            grid.at(i+j+10000*i) = 1.;
        }
        
    }

    GridWindowManager window(1000,1000,10000,10000);
    
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