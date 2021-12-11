#include <FluidGrid2.hpp>

#include <iostream>

#include "2DGraphics.hpp"
#include <vector>
#include <iterator>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

std::vector<float> flat(std::vector<std::vector<double>> grid){
        std::vector<float> acc;
        using std::begin;
        using std::end;
        for (auto vect : grid){
            acc.insert(end(acc), begin(vect), end(vect));
        }
        return acc;
    }

int main(int argc, char* argv[])
{
    using namespace std;
    int size = 9;

    FluidGrid2 grid { size, .01, 0.1 };
    
    cout << grid << endl;
    grid.add_density (Tuple<int> (2,1), 100.);
    grid.add_density (Tuple<int> (3,5), 1.);
    grid.add_density (Tuple<int> (8,8), 100.);

    cout << grid._sources;
    Grid vx = GridStruct(size).init();
    Grid vy = GridStruct(size).init();
    for (int i = 0; i < size; i++) 
    {
        for (int j = 0; j < size; j++)
        {
            vx[i][j] = 1.;
        }
    }
    //cout << vy << vx;
    grid.dens_step(Tuple<Grid> (vx, vy));
    cout << grid._densities;
    grid.dens_step(Tuple<Grid> (vx, vy));
    cout << grid._densities;
    
    

    /*std::vector<float> rid = 
    {1.,0.,0.,0.,0.,1.,
     0.,1.,0.,0.,0.,0.,
     0.,0.5,1.,0.,0.,0.05,
     0.,1.,0.,1.,0.,0.,
     0.,0.,0.,0.,1.,0.,
     0.,0.,0.,0.,0.,1.};*/

     std::vector<float> rid = 
    {1., 0., 0., 0., 0., 1., 0., 0., 0.,
     0., 1., 0., 0., 0., 0., 0., 0., 0.,
     0., 0.5, 1., 0., 0., 0.05, 0., 0., 0.,
     0., 1., 0., 1., 0., 0., 0., 0., 0.,
     0., 0., 0., 0., 1., 0., 0., 0., 0.,
     0., 0., 0., 0., 0., 1., 0., 0., 0., 
     0., 0., 0., 0., 0., 0., 1., 0., 0.,
     0., 0., 0., 0., 0., 0., 0., 1., 0.,
     0., 0., 0., 0., 0., 0., 0., 0., 1.};
    
    GridWindowManager window(500,500, size, size);
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
        window.display_grid<float>( flat(grid._sources), [](float i){ return Color(0,0,i);});
        //sleep(2);
        grid.dens_step(Tuple<Grid> (vx, vy));
        /*window.display_grid<float>( rid, [](float i){ return Color(0,0,i);});
        sleep(2);*/
        
    }
    return 0;
}






/*
// density check
cout << grid._densities << endl;

grid.addDensity(Tuple<int> (3, 3), 1.);
grid.addDensity(Tuple<int> (0, 0), 0.);
grid.addDensity(Tuple<int> (4, 4), 5.);

cout << grid._densities << endl;
*/

/*
// velocity check
cout << grid._Vx << endl;

grid.addVelocity(Tuple<int> (0, 0), Tuple<double> (3., 2.));
grid.addVelocity(Tuple<int> (4, 4), Tuple<double> (0., 5.));

cout << grid._Vx << endl;
cout << grid._Vy << endl;
*/