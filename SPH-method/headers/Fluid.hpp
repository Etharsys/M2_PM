#pragma once

#include <vector>
#include <Particle.hpp>

class Fluid
{
public:
    Fluid(int x_min, int x_max, int y_min, int y_max, float gap = H);

    Fluid(int nb_particles);

    Fluid(Eigen::Vector2d center, int radius, float gap = H);

    void compute_density_pressure();

    void compute_forces();

    void integrate();

    void update();

    constexpr static int WIDTH = 800;
    constexpr static int HEIGHT = 600;
    constexpr static float H = 16.f;                // kernel radius

private:
    
    std::vector<Particle> particles;

    // "Particle-Based Fluid Simulation for Interactive Applications" by Müller et al.
    // solver parameters
    static const Eigen::Vector2d G; // external (gravitational) forces
    constexpr static float REST_DENS = 300.f;       // rest density
    constexpr static float GAS_CONST = 2000.f;      // const for equation of state
    constexpr static float HSQ = H * H;             // radius^2 for optimization
    constexpr static float MASS = 2.5f;             // assume all particles have the same mass
    constexpr static float VISC = 200.f;            // viscosity constant
    constexpr static float DT = 0.0007f;            // integration timestep
    

    // smoothing kernels defined in Müller and their gradients
    // adapted to 2D per "SPH Based Shallow Water Simulation" by Solenthaler et al.
    const static float POLY6 ;
    const static float SPIKY_GRAD;
    const static float VISC_LAP;

    // simulation parameters
    constexpr static float EPS = H; // boundary epsilon
    constexpr static float BOUND_DAMPING = -0.5f;

    
};
