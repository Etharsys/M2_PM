#include <FluidGrid2.hpp>

#include <iostream>


int main(int argc, char* argv[])
{
    using namespace std;
    int size = 6;

    FluidGrid2 grid { size, .01, 0.1 };
    
    cout << grid << endl;
    grid.add_density (Tuple<int> (2,1), 100.);
    //grid.add_density (Tuple<int> (1,1), 100.);

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