#include <Fluid.hpp>

const Eigen::Vector2d Fluid::G {0.f, -10.f};
const float Fluid::POLY6 = 4.f / (M_PI * pow(Fluid::H, 8.f));
const float Fluid::SPIKY_GRAD = -10.f / (M_PI * pow(Fluid::H, 5.f));
const float Fluid::VISC_LAP = 40.f / (M_PI * pow(Fluid::H, 5.f));
constexpr float Fluid::MASS;
constexpr float Fluid::DT;

Fluid::Fluid(int x_min, int x_max, int y_min, int y_max, float gap)
{
    for (float x = x_min; x < x_max; x += gap)
    {
        for (float y = y_min; y < y_max; y += gap)
        {
            particles.emplace_back(x, y);
        }
    }
}

Fluid::Fluid(int nb_particles)
{
    for (int i = 0; i < nb_particles; i++)
    {
        particles.emplace_back(float(rand()) / RAND_MAX * (WIDTH - H), float(rand()) / RAND_MAX * (HEIGHT-H));
    }
}

Fluid::Fluid(Eigen::Vector2d center, int radius, float gap)
{
    for (float x = center.x() - radius; x < center.x() + radius; x += gap)
    {
        for (float y = center.y() - radius; y < center.y() + radius; y += gap)
        {
            if((Eigen::Vector2d{x,y}-center).norm() < radius )
            particles.emplace_back(x, y);
        }
    }
}

void Fluid::compute_density_pressure()
{
    for (Particle &particle_a : particles)
    {
        particle_a.density = 0.f;
        for (Particle &particle_b : particles)
        {
            Eigen::Vector2d vector_ab = particle_b.position - particle_a.position;
            float squared_distance = vector_ab.squaredNorm();
            if (squared_distance < HSQ)
            {
                particle_a.density += MASS * POLY6 * pow(HSQ - squared_distance, 3.f);
            }
        }
        particle_a.pressure = GAS_CONST * (particle_a.density - REST_DENS);
    }
}

void Fluid::compute_forces()
{
    for (Particle &particle_a : particles)
    {
        Eigen::Vector2d pressure_force(0.f, 0.f);
        Eigen::Vector2d viscosity_force(0.f, 0.f);
        for (Particle &particle_b : particles)
        {
            if (particle_a == particle_b)
            {
                continue;
            }
            Eigen::Vector2d vector_ab = particle_b.position - particle_a.position;
            float distance = vector_ab.norm();
            if (distance < H)
            {
                // compute pressure force contribution
                pressure_force += -vector_ab.normalized() * MASS * (particle_a.pressure + particle_b.pressure) / (2.f * particle_b.density) * SPIKY_GRAD * pow(H - distance, 3.f);
                // compute viscosity force contribution
                viscosity_force += VISC * MASS * (particle_b.velocity - particle_a.velocity) / particle_b.density * VISC_LAP * (H - distance);
            }
        }
        Eigen::Vector2d gravity_force = G * MASS / particle_a.density;
        particle_a.force = pressure_force + viscosity_force + gravity_force;
    }
}

void Fluid::integrate()
{
    #pragma omp parallel for
    for (auto &particle : particles)
    {
        // forward Euler integration
        particle.velocity += DT * particle.force / particle.density;
        particle.position += DT * particle.velocity;
        // enforce boundary conditions
        if (particle.position.x() - EPS < 0.f)
        {
            particle.velocity.x() *= BOUND_DAMPING;
            particle.position.x() = EPS;
        }
        if (particle.position.x() + EPS > WIDTH)
        {
            particle.velocity.x() *= BOUND_DAMPING;
            particle.position.x() = WIDTH - EPS;
        }
        if (particle.position.y() - EPS < 0.f)
        {
            particle.velocity.y() *= BOUND_DAMPING;
            particle.position.y() = EPS;
        }
        if (particle.position.y() + EPS > HEIGHT)
        {
            particle.velocity.y() *= BOUND_DAMPING;
            particle.position.y() = HEIGHT - EPS;
        }
    }
}

void Fluid::update()
{
    compute_density_pressure();
    compute_forces();
    integrate();
}
