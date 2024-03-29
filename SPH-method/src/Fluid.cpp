#include <Fluid.hpp>

const Eigen::Vector2d Fluid::G{0.f, -10.f};
const float Fluid::POLY6 = 4.f / (M_PI * pow(Fluid::H, 8.f));
const float Fluid::SPIKY_GRAD = -10.f / (M_PI * pow(Fluid::H, 5.f));
const float Fluid::VISC_LAP = 40.f / (M_PI * pow(Fluid::H, 5.f));
constexpr float Fluid::MASS;
constexpr float Fluid::DT;
const float Fluid::MASS_X_POLY6 = MASS * POLY6;
const Eigen::Vector2d Fluid::G_X_MASS = G * MASS;
const float Fluid::VISC_X_MASS = VISC * MASS;
const float Fluid::MASS_X_POLY6_X_POW0 = MASS_X_POLY6 * pow(HSQ, 3.f);

Fluid::Fluid(const int x_min, const int x_max, const int y_min, const int y_max, const float gap)
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

Fluid::Fluid(const Eigen::Vector2d center, const int radius, const float gap)
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
    //#pragma omp parallel for num_threads(8)
    for (int i = 0; i < particles.size(); i++)
    {
        auto surrounding_particles = particles.get_surrounding_elements(i);
        for (auto particle_a = particles[i]; particle_a!=nullptr;particle_a = particle_a->next )
        {
            #pragma omp parallel for num_threads(8)
            for (auto particle_b : surrounding_particles)
            {
                for (; particle_b!=nullptr;particle_b = particle_b->next)
                {
                    if(!particle_b->processed_density)
                    {
                        Eigen::Vector2d vector_ab = particle_b->position - particle_a->position;
                        float squared_distance = vector_ab.squaredNorm();
                        if (squared_distance < HSQ)
                        {
                            float density;
                            if(squared_distance == 0)
                            {
                                density = MASS_X_POLY6_X_POW0;
                            }
                            else
                            {
                                density = MASS_X_POLY6 * pow(HSQ - squared_distance, 3.f);
                            }
                            #pragma omp critical
                            particle_a->density += density;
                            if(particle_a!=particle_b)
                            {
                                #pragma omp critical
                                particle_b->density += density;
                            }
                        }
                    }
                }
            }
            particle_a->processed_density = true;
            particle_a->pressure = GAS_CONST * (particle_a->density - REST_DENS);
        }
    }
}

void Fluid::compute_forces()
{
    for (int i = 0; i < particles.size(); i++)
    {
        auto surrounding_particles = particles.get_surrounding_elements(i);
        for (auto particle_a = particles[i]; particle_a!=nullptr;particle_a = particle_a->next )
        {
            #pragma omp parallel for num_threads(8)
            for (auto particle_b : surrounding_particles)
            {
                for (; particle_b!=nullptr;particle_b = particle_b->next)
                {
                    if(!particle_b->processed_force)
                    {
                        if (*particle_a == *particle_b)
                        {
                            continue;
                        }
                        Eigen::Vector2d vector_ab = particle_b->position - particle_a->position;
                        float distance = vector_ab.norm();
                        vector_ab = vector_ab.normalized();
                        if (distance < H)
                        {
                            float pressure = MASS * (particle_a->pressure + particle_b->pressure);
                            float viscosity_coef  = VISC_LAP * (H - distance);
                            float pressure_coef = SPIKY_GRAD * pow(H - distance, 3.f);
                            // compute pressure force contribution
                            #pragma omp critical
                            particle_a->force += -vector_ab * pressure / (2.f * particle_b->density) * pressure_coef;
                            // compute viscosity force contribution
                            #pragma omp critical
                            particle_a->viscosity += VISC_X_MASS * (particle_b->velocity - particle_a->velocity) / particle_b->density * viscosity_coef;
                            // compute pressure force contribution
                            #pragma omp critical
                            particle_b->force += vector_ab * pressure / (2.f * particle_a->density) * pressure_coef;
                            // compute viscosity force contribution
                            #pragma omp critical
                            particle_b->viscosity += VISC_X_MASS * (particle_a->velocity - particle_b->velocity) / particle_a->density * viscosity_coef;
                        }
                    }
                }
            }
            particle_a->processed_force = true;
            Eigen::Vector2d gravity_force = G_X_MASS / particle_a->density;
            particle_a->force = particle_a->force + particle_a->viscosity + gravity_force;
        }
    }
}

void Fluid::integrate()
{
    #pragma omp parallel for num_threads(8)
    for (auto &particle : particles.get_all_elements())
    {   
        // forward Euler integration
        particle.velocity += DT * particle.force / particle.density;
        Eigen::Vector2d newPositon = particle.position + DT * particle.velocity;
        particle.processed_density = false;
        particle.processed_force =false;
        particle.density = 0.f;
        particle.force[0] = 0.f;
        particle.force[1] = 0.f;
        particle.viscosity[0] = 0.f;
        particle.viscosity[1] = 0.f;
        // enforce boundary conditions
        if (newPositon.x() - H < 0.f)
        {
            particle.velocity.x() *= BOUND_DAMPING;
            newPositon.x() = H;
        }
        if (newPositon.x() + H > WIDTH)
        {
            particle.velocity.x() *= BOUND_DAMPING;
            newPositon.x() = WIDTH - H;
        }
        if (newPositon.y() - H < 0.f)
        {
            particle.velocity.y() *= BOUND_DAMPING;
            newPositon.y() = H;
        }
        if (newPositon.y() + H > HEIGHT)
        {
            particle.velocity.y() *= BOUND_DAMPING;
            newPositon.y() = HEIGHT - H;
        }
        #pragma omp critical
        particles.move_element(newPositon.x(), newPositon.y(), &particle);
    }
}

void Fluid::update()
{
    compute_density_pressure();
    compute_forces();
    integrate();
}
