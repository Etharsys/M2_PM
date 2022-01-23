#include <SPH/2D/Fluid2D.hpp>

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
            particles.emplace_back(x, y);
        }
    }
    particles_grid.sort_grid(particles);
}

Fluid::Fluid(const int nb_particles)
{
    for (int i = 0; i < nb_particles; i++)
    {
        float x = float(rand()) / RAND_MAX * (WIDTH - 2 * H) + H;
        float y = float(rand()) / RAND_MAX * (HEIGHT - 2 * H) + H;
        particles.emplace_back(x, y);
    }
    particles_grid.sort_grid(particles);
}

Fluid::Fluid(const Eigen::Vector2d center, const int radius, const float gap)
{
    for (float x = center.x() - radius; x < center.x() + radius; x += gap)
    {
        for (float y = center.y() - radius; y < center.y() + radius; y += gap)
        {
            if ((Eigen::Vector2d{x, y} - center).norm() < radius)
            {
                particles.emplace_back(x, y);
            }
        }
    }
    particles_grid.sort_grid(particles);
}

void Fluid::compute_density_pressure()
{
    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < particles_grid.size(); i++)
    {
        auto surrounding_particles = particles_grid.get_surrounding_elements(i);
        for (auto particle_a = particles_grid[i]; particle_a != nullptr; particle_a = particle_a->next)
        {
            particle_a->density = 0;
            for (auto particle_b : surrounding_particles)
            {
                for (; particle_b != nullptr; particle_b = particle_b->next)
                {
                    Eigen::Vector2d vector_ab = particle_b->position - particle_a->position;
                    float squared_distance = vector_ab.squaredNorm();
                    if (squared_distance < HSQ)
                    {
                        particle_a->density += MASS_X_POLY6 * pow(HSQ - squared_distance, 3.f);
                    }
                }
            }
            particle_a->pressure = GAS_CONST * (particle_a->density - REST_DENS);
        }
    }
}

void Fluid::compute_forces()
{
    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < particles_grid.size(); i++)
    {
        auto surrounding_particles = particles_grid.get_surrounding_elements(i);
        for (auto particle_a = particles_grid[i]; particle_a != nullptr; particle_a = particle_a->next)
        {
            Eigen::Vector2d pressure_force(0.f,0.f);
            Eigen::Vector2d viscosity_force(0.f,0.f);
            for (auto particle_b : surrounding_particles)
            {
                for (; particle_b != nullptr; particle_b = particle_b->next)
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
                        float viscosity_coef = VISC_LAP * (H - distance);
                        float pressure_coef = SPIKY_GRAD * pow(H - distance, 3.f);
                        pressure_force += -vector_ab * pressure / (2.f * particle_b->density) * pressure_coef;
                        viscosity_force += VISC_X_MASS * (particle_b->velocity - particle_a->velocity) / particle_b->density * viscosity_coef;
                    }
                }
            }
            Eigen::Vector2d gravity_force = G_X_MASS / particle_a->density;
            particle_a->force = pressure_force + viscosity_force + gravity_force;
        }
    }
}

void Fluid::integrate()
{
    #pragma omp parallel for
    for (auto &particle : particles)
    {
        // forward Euler integration
        particle.velocity += DT * particle.force / particle.density;
        Eigen::Vector2d newPositon = particle.position + DT * particle.velocity;
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
        particles_grid.move_element(newPositon.x(), newPositon.y(), &particle);
    }
}

void Fluid::update()
{
    compute_density_pressure();
    compute_forces();
    integrate();
}

std::vector<float> Fluid::render_top_surface(int size)
{
    std::vector<float> buffer(size, 0);
    std::vector<float> color(size, 0);
    float pixel_size = WIDTH / float(size);
    for (Particle &particle : particles)
    {
        float x = particle.position.x();
        float y = particle.position.y();
        float left = x - H;
        float right = x + H;
        int left_index = int(left / pixel_size);
        int right_index = int(right / pixel_size);
        for (; left_index <= right_index; left_index++)
        {
            float left_bound = left_index * pixel_size;
            float right_bound = (left_index + 1) * pixel_size;
            float value = 0;
            if (left_bound < x && right_bound > x)
            {
                value = y + H;
            }
            else if (left_bound < x)
            {
                value = y + (x - left_bound);
            }
            else
            {
                value = y + (right_bound - x);
            }
            if (value > buffer[left_index])
            {
                buffer[left_index] = value;
            }
        }
    }
    int kernel_size = int(H / pixel_size);
    for (int i = kernel_size; i < size - kernel_size; i++)
    {
        int n = 0;
        float sum = 0;
        for (int j = i - kernel_size; j < i + kernel_size; j++)
        {
            if (abs(buffer[j] - buffer[i]) < H)
            {
                sum += buffer[j];
                n++;
            }
        }
        color[i] = sum / n / HEIGHT;
    }
    return color;
}

const Grid<Particle> &Fluid::get_grid()
{
    return particles_grid;
}