// Standard
#include <cstdlib>
#include <vector>
#include <iostream>

// Project
#include <Color.hpp>
#include <ColorGradient.hpp>
#include <ShadersManager.hpp>
#include <2DGraphics.hpp>

/* macros */

#define IX(i, j) ((i)+(N+2)*(j))

extern void dens_step(int N, float *x, float *x0, float *u, float *v, float diff, float dt);

extern void vel_step(int N, float *u, float *v, float *u0, float *v0, float viscosity, float dt);

struct Config
{
    Config(int n, int ts, float dt, float diff, float visc, float force, float source) :
            N(n),
            Nf(static_cast<float>(N)),
            Ts(ts),
            dt(dt),
            diff(diff),
            visc(visc),
            force(force),
            source(source)
    {
    }

    int size() const
    {
        return (N + 2) * (N + 2);
    }

    int N;
    int Ts;
    float Nf;
    float dt, diff, visc;
    float force, source;
};

struct Event
{
    explicit Event(int size) :
            win_x(size),
            win_y(size)
    {
    }

    Event(int winX, int winY) :
            win_x(winX),
            win_y(winY)
    {
    }

    void handleEvent(const SDL_Event &e)
    {
        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            omx = mx = e.button.x;
            omy = my = e.button.y;
            if (e.button.button == SDL_BUTTON_LEFT)
            {
                mouse_down[0] = 1;
            }
            if (e.button.button == SDL_BUTTON_RIGHT)
            {
                mouse_down[2] = 1;
            }
        }
        else if (e.type == SDL_MOUSEBUTTONUP)
        {
            if (e.button.button == SDL_BUTTON_LEFT)
            {
                mouse_down[0] = 0;
            }
            if (e.button.button == SDL_BUTTON_RIGHT)
            {
                mouse_down[2] = 0;
            }
        }
        else if (e.type == SDL_MOUSEMOTION)
        {
            mx = e.motion.x;
            my = e.motion.y;
        }
    }

    int win_x, win_y;
    int mouse_down[3]{0, 0, 0};
    int omx{0}, omy{0}, mx{0}, my{0};
};

struct Data
{
    explicit Data(int N) : N{N}
    {
        int size = (N + 2) * (N + 2);

        u = (float *) malloc(size * sizeof(float));
        v = (float *) malloc(size * sizeof(float));
        u_prev = (float *) malloc(size * sizeof(float));
        v_prev = (float *) malloc(size * sizeof(float));
        dens = (float *) malloc(size * sizeof(float));
        dens_prev = (float *) malloc(size * sizeof(float));

        if (!u || !v || !u_prev || !v_prev || !dens || !dens_prev)
        {
            throw std::exception();
        }
    }

    ~Data()
    {
        if (u)
        {
            free(u);
        }
        if (v)
        {
            free(v);
        }
        if (u_prev)
        {
            free(u_prev);
        }
        if (v_prev)
        {
            free(v_prev);
        }
        if (dens)
        {
            free(dens);
        }
        if (dens_prev)
        {
            free(dens_prev);
        }
    }

    void clear() const
    {
        int i, size = (N + 2) * (N + 2);

        for (i = 0; i < size; i++)
        {
            u[i] = v[i] = u_prev[i] = v_prev[i] = dens[i] = dens_prev[i] = 0.0f;
        }
    }

    std::vector<float> flat() const
    {
        std::vector<float> new_data;
        int idx = 0;
        for (auto j = 1; j <= N; j++)
        {
            for (auto i = 1; i <= N; i++)
            {
                new_data.push_back(dens[IX(i, j)]);
                idx++;
            }
        }
        return new_data;
    }

    void get_from_UI(Event &event, const Config &config) const
    {
        int i, j;

        for (i = 0; i < config.size(); i++)
        {
            u_prev[i] = v_prev[i] = dens_prev[i] = 0.0f;
        }

        if (!event.mouse_down[0] && !event.mouse_down[2])
        {
            return;
        }

        i = static_cast<int>((static_cast<float>(event.mx) / static_cast<float>(event.win_x)) * config.Nf) + 1;
        j = static_cast<int>((static_cast<float>(event.my) / static_cast<float>(event.win_y)) * config.Nf) + 1;

        if (i < 1 || i > N || j < 1 || j > N)
        {
            return;
        }

        if (event.mouse_down[0])
        {
            u_prev[IX(i, j)] = config.force * static_cast<float>(event.mx - event.omx);
            v_prev[IX(i, j)] = config.force * static_cast<float>(event.my - event.omy);
        }

        if (event.mouse_down[2])
        {
            dens_prev[IX(i, j)] = config.source;
        }

        event.omx = event.mx;
        event.omy = event.my;
    }

    int N;
    float *u, *v, *u_prev, *v_prev;
    float *dens, *dens_prev;
};

/*
  ----------------------------------------------------------------------
   main
  ----------------------------------------------------------------------
*/

// Debug
void print_array(float *array, int N)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            std::cout << array[IX(i, j)] << "|";
        }
        std::cout << std::endl;
    }
}

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
    Config config(N, 5, .1, 0, .0001, 5, 100);
    Event event(N * config.Ts);
    Data data(N);

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
                if (e.key.keysym.sym == SDLK_a)
                {
                    print_array(data.dens, N);
                }
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