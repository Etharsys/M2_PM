#pragma once

#include <vector>
#include <SPH/2D/Particle2D.hpp>
#include <unordered_set>
#include <SPH/2D/Grid2D.hpp>

/*
References :
https://en.wikibooks.org/wiki/Algorithm_Implementation/Geometry/Convex_hull/Monotone_chain
https://lucasschuermann.com/writing/implementing-sph-in-2d
https://matthias-research.github.io/pages/publications/sca03.pdf
http://www.danenglesson.com/images/portfolio/FLIP/rapport.pdf
https://github.com/tizian/SPH-Water-Simulation
https://jcgt.org/published/0007/01/02/paper-lowres.pdf
*/

/*  @brief Particle based fluid respecting navier stokes equations
 */
class Fluid
{
public:
    /* @brief Initialize fluid particles into rectangle with gap distance
     *  @param x_min x_min coordinate
     *  @param x_max x_max coordinate
     *  @param y_min y_min coordinate
     *  @param x_min y_max coordinate
     *  @param gap Distance between each particle
     */
    Fluid(const int x_min, const int x_max, const int y_min, const int y_max, const float gap = H);

    /* @brief Initialize fluid particles randomly
     *  @param nb_particles the number of particles
     */
    Fluid(const int nb_particles);

    /* @brief Initialize fluid particles into a circle
     *  @param center circle center
     *  @param radius circle radius
     *  @param gap Distance between each particle
     */
    Fluid(const Eigen::Vector2d center, const int radius, const float gap = H);

    /* @brief Compute density and pressure for each particle */
    void compute_density_pressure();

    /* @brief Compute forces for each particle */
    void compute_forces();

    /* @brief Update position and velocity for each particle*/
    void integrate();

    /* @brief Update each particle properties*/
    void update();

    /* @brief Particle splatting rendering on top of fluid 
    *  @return surface color buffer
    */
    std::vector<float> render_top_surface(int size);

    /* @brief get the fluid grid
    *  @return grid system
    */
    const Grid<Particle>& get_grid();

    /*window width*/
    constexpr static int WIDTH = 800;
    /*window height*/
    constexpr static int HEIGHT = 600;
    
private:
    /*particle radius*/
    constexpr static float H = 16.f;
    /*grid of particles*/
    Grid<Particle> particles_grid{H * 2, 0, WIDTH, 0, HEIGHT};
    /*list of elements*/
    std::vector<Particle> particles;
    /*gravitation vector*/
    static const Eigen::Vector2d G;
    /*rest density*/
    constexpr static float REST_DENS = 300.f;
    /*GAS constant*/
    constexpr static float GAS_CONST = 2000.f;
    /* radius² */
    constexpr static float HSQ = H * H;
    /* particle mass */
    constexpr static float MASS = 2.5f;
    /* viscosity */
    constexpr static float VISC = 200.f;
    /* time step */
    constexpr static float DT = 0.0007f;

    /*Poly6 kernel*/
    const static float POLY6;
    /*gradient of spiky kernel*/
    const static float SPIKY_GRAD;
    /*laplacian of viscosity kernel*/
    const static float VISC_LAP;
    /*MASS * POLY6 value*/
    const static float MASS_X_POLY6;
    /*G * MASS vector*/
    const static Eigen::Vector2d G_X_MASS;
    /*VISC * MASS value*/
    const static float VISC_X_MASS;
    /*MASS * POLY6 * pow(HSQ)*/
    const static float MASS_X_POLY6_X_POW0;

    /* bound damping */
    constexpr static float BOUND_DAMPING = -0.5f;
};
