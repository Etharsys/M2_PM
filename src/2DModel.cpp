// Standard
#include <cstdlib>

// Project
#include <Color.hpp>
#include <2DGraphics.hpp>
#include <ColorGradient.hpp>
#include <fluid2d/Config.hpp>
#include <fluid2d/Event.hpp>
#include <fluid2d/Data.hpp>

/* macros */

extern void dens_step(int N, float *x, float *x0, float *u, float *v, float diff, float dt);

extern void vel_step(int N, float *u, float *v, float *u0, float *v0, float viscosity, float dt);

/*
 * Je l'aurais bien mis en champs du main mais j'arrive pas à créer le lambda du coup..
 * Ca marche pas ça:
 * [&capture](float d){return colors.get_color(d);}
 *
 * Donc pour l'instant ça reste en static
 */
static ColorGradient colors{
        Color{.00, .00, .00},
        Color{.11, .67, .00},
        Color{.61, 1.0, .25},
        Color{1.0, 1.0, .37},
        Color{.93, 1.0, .91}
};

//static ColorGradient colors{
//        Color{0.0, 0.0, 0.0},
//        Color{.14, .14, .77},
//        Color{.46, .96, 1.0},
//        Color{.93, .37, 1.0},
//        Color{1.0, .91, 1.0}
//};

int main()
{
    int N{128};
    fluid2d::Config config(N, 5, .1, 0, .0001, 5, 100);
    fluid2d::Event event(N * config.Ts);
    fluid2d::Data data(N);

    // Application loop:
    GridWindowManager window(event.win_x, event.win_y, N, N);
    bool done = false;
    while (!done)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                done = true; // Leave the loop after this iteration
            }
            if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_c)
                {
                    data.clear();
                }
                if (e.key.keysym.sym == SDLK_v)
                {
                    config.visc += 0.0001f;
                }
            }
            event.handleEvent(e);
        }
        data.get_from_UI(event, config);

        vel_step(N, data.u, data.v, data.u_prev, data.v_prev, config.visc, config.dt);
        dens_step(N, data.dens, data.dens_prev, data.u, data.v, config.diff, config.dt);
        window.display_grid<float>(data.flat(), [](float d)
        {
            return colors.get_color(d);
        });
    }

    return EXIT_SUCCESS;
}