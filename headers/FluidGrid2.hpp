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
        /**
         * @brief : simulation of a grid where each cells represent a fluid location
         * @arg size : the size of the grid
         * @arg diffusion : coefficient of the fluid diffusion capacity
         * @arg lifespan : life time of the fluid since it started ? 
         */
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

        
        /**
         * @brief : add density at a specific location int the grid
         * @arg coords : coordinates of the grid to change density
         * @arg amount : value of the density to add 
         */
        void add_density(Tuple<int> coords, double amount);

        /**
         * @brief : modify the _densities grid to correspond to a _sources grid
         */
        void add_source();

        /**
         * @brief : compute the edges & corners of a grid
         * @arg bound : size of the boundary ???
         * @arg x : the grid to compute
         */
        void set_bnd(int bound, Grid& x);

        /**
         * @brief : diffuse the fluid to a grid
         * @arg bound : size of the boundary ???
         * @arg x  : the grid which need to be diffuse
         * @arg x0 : a grid with the previous status of x
         * @arg iter : the number of iteration we need to diffuse 
         */
        void diffuse(int bound, Grid& x, const Grid& x0, int iter);

        /**
         * @brief : modify _densities be following velocities grids 
         * @arg bound : size of the boundary ???
         * @arg d  : the grid which need to be diffuse
         * @arg d0 : a grid with the previous status of d
         * @arg velocities : 2 grids of velocities vX and vY
         */
        void advect(int bound, Grid& d, const Grid& d0, const Tuple<Grid>& velocities);

        /**
         * @brief : do one step to compute densities operations
         * @arg vel : two fixed grid of velocities for the advect operation 
         */
        void dens_step(const Tuple<Grid>& vel);

        /**
         * @brief : shell display
         */
        friend std::ostream& operator<<(std::ostream& os, FluidGrid2& fc);

        /**
         * @brief : side of the grid
         * @return : return the length of a side of the (cubical) grid
         */
        int side() { return _size; }

        /**
         * @brief : size of the grid
         * @return : return the number of cells of the grid (side * side)
         */
        int size() { return _size * _size; }




    public:
        int    _size;
        double _diffusion;
        double _lifespan;

        Grid _sources;
        Grid _densities;
};