#include <Fluid.hpp>

const Eigen::Vector2d Fluid::G{0.f, -10.f};
const float Fluid::POLY6 = 4.f / (M_PI * pow(Fluid::H, 8.f));
const float Fluid::SPIKY_GRAD = -10.f / (M_PI * pow(Fluid::H, 5.f));
const float Fluid::VISC_LAP = 40.f / (M_PI * pow(Fluid::H, 5.f));
constexpr float Fluid::MASS;
constexpr float Fluid::DT;

Fluid::Fluid(const int x_min,const int x_max,const int y_min,const int y_max,const float gap)
{
    for (float x = x_min; x < x_max; x += gap)
    {
        for (float y = y_min; y < y_max; y += gap)
        {
            particles.add_element(x, y);
        }
    }
    particles.sort_grid();
}

Fluid::Fluid(const int nb_particles)
{
    for (int i = 0; i < nb_particles; i++)
    {
        float x = float(rand()) / RAND_MAX * (WIDTH - H);
        float y = float(rand()) / RAND_MAX * (HEIGHT - H);
        particles.add_element(x, y);
    }
    particles.sort_grid();
}

Fluid::Fluid(const Eigen::Vector2d center,const int radius,const float gap)
{
    for (float x = center.x() - radius; x < center.x() + radius; x += gap)
    {
        for (float y = center.y() - radius; y < center.y() + radius; y += gap)
        {
            if ((Eigen::Vector2d{x, y} - center).norm() < radius)
            {
                particles.add_element(x, y);
            }
        }
    }
    particles.sort_grid();
}

void Fluid::compute_density_pressure()
{
    for (auto &particle_a : particles.get_all_elements())
    {
        particle_a.density = 0.f;
        for (auto &particle_b : particles.get_surrounding_elements(particle_a.position.x(), particle_a.position.y()))
        {

            Eigen::Vector2d vector_ab = particle_b->position - particle_a.position;
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
    for (auto &particle_a : particles.get_all_elements())
    {
        Eigen::Vector2d pressure_force(0.f, 0.f);
        Eigen::Vector2d viscosity_force(0.f, 0.f);
        for (auto &particle_b : particles.get_surrounding_elements(particle_a.position.x(), particle_a.position.y()))
        {
            
            if (particle_a == *particle_b)
            {
                continue;
            }
            Eigen::Vector2d vector_ab = particle_b->position - particle_a.position;
            float distance = vector_ab.norm();
            if (distance < H)
            {
                // compute pressure force contribution
                pressure_force += -vector_ab.normalized() * MASS * (particle_a.pressure + particle_b->pressure) / (2.f * particle_b->density) * SPIKY_GRAD * pow(H - distance, 3.f);
                // compute viscosity force contribution
                viscosity_force += VISC * MASS * (particle_b->velocity - particle_a.velocity) / particle_b->density * VISC_LAP * (H - distance);
            }
        }
        Eigen::Vector2d gravity_force = G * MASS / particle_a.density;
        particle_a.force = pressure_force + viscosity_force + gravity_force;
    }
}

void Fluid::integrate()
{
    for (auto &particle : particles.get_all_elements())
    {
        // forward Euler integration
        particle.velocity += DT * particle.force / particle.density;
        Eigen::Vector2d newPositon = particle.position + DT * particle.velocity;
        // enforce boundary conditions
        if (newPositon.x() - EPS < 0.f)
        {
            particle.velocity.x() *= BOUND_DAMPING;
            newPositon.x() = EPS;
        }
        if (newPositon.x() + EPS > WIDTH)
        {
            particle.velocity.x() *= BOUND_DAMPING;
            newPositon.x() = WIDTH - EPS;
        }
        if (newPositon.y() - EPS < 0.f)
        {
            particle.velocity.y() *= BOUND_DAMPING;
            newPositon.y() = EPS;
        }
        if (newPositon.y() + EPS > HEIGHT)
        {
            particle.velocity.y() *= BOUND_DAMPING;
            newPositon.y() = HEIGHT - EPS;
        }
        particles.move_element(newPositon.x(),newPositon.y(),particle);
    }
}

void Fluid::update()
{
    compute_density_pressure();
    compute_forces();
    integrate();
}
