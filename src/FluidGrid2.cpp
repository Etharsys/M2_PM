#include <FluidGrid2.hpp>

#include <cmath>
#include <algorithm>



void FluidGrid2::add_density(Tuple<int> coords, double amount)
{
    _sources[coords.x][coords.y] += amount;
}

void FluidGrid2::add_source()
{
    for (int i = 0; i < _size; i++)
    {
        for (int j = 0; j < _size; j++)
        {
            _densities[i][j] += _lifespan * _sources[i][j];
        }
    }
}


void FluidGrid2::set_bnd(int bound, Grid& x)
{
    //std::cout << x;
    //std::cout << x[0][2] << std::endl;
    for (int i = 1; i <= side(); ++i)
    {
        x[0       ][i] = bound == 1 ? -x[1     ][i] : x[1     ][i];
        x[side()+1][i] = bound == 1 ? -x[side()][i] : x[side()][i];
        x[i][0       ] = bound == 2 ? -x[i][1     ] : x[i][1     ];
        x[i][side()+1] = bound == 2 ? -x[i][side()] : x[i][side()];
    }

    x[0       ][0       ] = .5 * (x[1     ][0       ] + x[0       ][1     ]);
    x[0       ][side()+1] = .5 * (x[1     ][side()+1] + x[0       ][side()]);
    x[side()+1][0       ] = .5 * (x[side()][0       ] + x[side()+1][1     ]);
    x[side()+1][side()+1] = .5 * (x[side()][side()+1] + x[side()+1][side()]);
}


void FluidGrid2::diffuse(int bound, Grid& x, const Grid& x0, int iter)
{
    double a = _lifespan * _diffusion * in_cells_n();

    for (int k = 0; k < iter; k++ ) 
    {
        for (int i = 1; i <= side(); i++ ) 
        {
            for (int j = 1 ; j <= side(); j++ ) 
            {
                auto tmp = x0[i-1][j] + x0[i+1][j] + x0[i][j-1] + x0[i][j+1];
                x[i][j]  = (x0[i][j] + a*tmp) / (1+4*a);
            }
        }
        set_bnd (bound, x);
    }
}


void FluidGrid2::advect(int bound, Grid& d, const Grid& d0, const Tuple<Grid>& velocities)
{
    Grid u = velocities.x;
    Grid v = velocities.y;
    double ls0 = _lifespan * side();
    for (int i = 1; i <= side(); i++)
    {
        for (int j = 1; j <= side(); j++)
        {
            double x = i - ls0 * u[i][j];
            double y = j - ls0 * v[i][j];

            if (x < .5) x = .5;
            if (x > side()+.5) x = side()+.5;
            int i0 = x;
            int i1 = i0 + 1;

            if (y < .5) y = .5;
            if (y > side()+.5) y = side()+.5;
            int j0 = y;
            int j1 = j0 + 1;

            double s1 = x - i0;
            double s0 = 1 - s1;
            double t1 = y - j0;
            double t0 = 1 - t1;

            d[i][j] = s0 * (t0 * d0[i0][j0] + t1 * d0[i0][j1]) + 
                    + s1 * (t0 * d0[i1][j0] + t1 * d0[i1][j1]);
        }
    }
    set_bnd(bound, d);
}


void FluidGrid2::dens_step(const Tuple<Grid>& vel)
{
    add_source();

    std::swap(_sources, _densities);
    diffuse(0, _densities, _sources, 20);

    std::swap(_sources, _densities);
    advect(0, _densities, _sources, vel);
}

std::ostream& operator<<(std::ostream& os, const Grid& grid)
{
    os << "--- GRID --- " << std::endl;
    for (auto line : grid)
    {
        for (auto cell : line)
        {
            os << cell << " ";
            //os << ceilf(cell) << "          ";
        }
        os << std::endl;
    }
    return os;
}


std::ostream& operator<<(std::ostream& os, FluidGrid2& fc)
{
    os << "=== FLUIDGRID === " << std::endl;
    os << "side length : " << fc._size << " | ";
    os << "side without borders : " << fc.side() << " | ";
    os << "nb cells : " << fc.cells_n() << " | ";
    os << "diffusion : " << fc._diffusion << " | ";
    //os << "viscosity : " << fc._viscosity << " | ";
    os << "lifespan : " << fc._lifespan << std::endl;
    //os << fc._s;
    return os;
}
