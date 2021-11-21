#pragma once


#include <vector>
#include <iostream>


using Grid = std::vector<std::vector<double>>;


struct GridStruct
{
    GridStruct (int size)
        : grid { Grid (size) }, _size { size }
    { }

    Grid init()
    {
        for (int i = 0; i < _size; ++i)
        {
            grid[i] = std::vector<double>(_size);
        }
        return grid;
    }


    Grid grid;
    int _size;
};

std::ostream& operator<<(std::ostream& os, const Grid& fd);


template <typename T>
struct Tuple
{
    Tuple (T x, T y)
        : x { x }, y { y }
    { }

    T x;
    T y;
};


class FluidGrid2
{
    public:
        FluidGrid2 (int    size,
                   double diffusion,
                   double lifespan)
            : _size      { size },
              _diffusion { diffusion },
              _lifespan  { lifespan }
        { 
            _sources   = GridStruct(size).init(); 
            _densities = GridStruct(size).init();
        }

        
        void add_density(Tuple<int> coords, double amount);

        void add_source();

        void set_bnd(int bound, Grid& x);

        void diffuse(int bound, Grid& x, const Grid& x0, int iter);

        void advect(int bound, Grid& d, const Grid& d0, const Tuple<Grid>& velocities);

        void dens_step(const Tuple<Grid>& vel);


        friend std::ostream& operator<<(std::ostream& os, FluidGrid2& fc);

        int side() { return _size; }

        int size() { return _size * _size; }




    public:
        int    _size;
        double _diffusion;
        double _lifespan;

        Grid _sources;
        Grid _densities;
};