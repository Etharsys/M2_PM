#include <2DGraphics.hpp>
#include <Fluid.hpp>

int main(int argc, char const *argv[])
{

    Fluid fluid{1000};

    GridWindowManager window(fluid.WIDTH, fluid.HEIGHT, fluid.particles.get_nb_columns(), fluid.particles.get_nb_row(), "SPH graphics");

    bool done = false;
    while (!done)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                done = true;
            }
        }

        window.display_grid<Particle *>(fluid.particles.get_grid(),
                                        [](Particle *particle)
                                        {
                                            int nbParticles = 0;
                                            for (Particle *next_particle = particle; next_particle != nullptr; next_particle = next_particle->next)
                                            {
                                                nbParticles++;
                                            }
                                            return Color(0, 0, nbParticles / 10.);
                                        });
        fluid.update();
    }
    return 0;
}