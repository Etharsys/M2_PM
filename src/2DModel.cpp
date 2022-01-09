// Standard
#include <cstdlib>

// Project
#include <Color.hpp>
#include <2DGraphics.hpp>
#include <ColorGradient.hpp>
#include <fluid2d/Event.hpp>
#include <fluid2d/Data.hpp>

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
    int TileSize{5};
    fluid2d::Event event(N * TileSize, 5);
    fluid2d::Data data(N, .1, 0, .0001, 100);

    // Application loop:
    GridWindowManager window(event.winX(), event.winY(), N, N);
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
                    data.update_viscosity(0.0001f);
                }
            }
            event.handleEvent(e);
        }
        data.get_from_UI(event);

        data.vel_step();
        data.dens_step();

        window.display_grid<float>(data.flat(), [](float d)
        {
            return colors.get_color(d);
        });
    }

    return EXIT_SUCCESS;
}