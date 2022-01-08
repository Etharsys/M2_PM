// Standard
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <iostream>

// OpenGL
#include <GL/glew.h>

// Project
#include <Color.hpp>
#include <ShadersManager.hpp>
#include <2DGraphics.hpp>

/* macros */

#define IX(i, j) ((i)+(N+2)*(j))

/* external definitions (from solver.c) */

extern void dens_step(int N, float *x, float *x0, float *u, float *v, float diff, float dt);

extern void vel_step(int N, float *u, float *v, float *u0, float *v0, float viscosity, float dt);

/* global variables */

// Config
static int N;
static int Ts; // Tile size
static float Nf;
static float dt, diff, visc;
static float force, source;

// Data
static float *u, *v, *u_prev, *v_prev;
static float *dens, *dens_prev;

// Event
static int win_x, win_y;
static int mouse_down[3];
static int omx, omy, mx, my;


/*
  ----------------------------------------------------------------------
   free/clear/allocate simulation data
  ----------------------------------------------------------------------
*/

static void free_data()
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

static void clear_data()
{
    int i, size = (N + 2) * (N + 2);

    for (i = 0; i < size; i++)
    {
        u[i] = v[i] = u_prev[i] = v_prev[i] = dens[i] = dens_prev[i] = 0.0f;
    }
}

static int allocate_data()
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
        fprintf(stderr, "cannot allocate data\n");
        return (0);
    }

    return (1);
}

/*
  ----------------------------------------------------------------------
   Pretty colors
  ----------------------------------------------------------------------
*/

float min(float a, float b)
{
    return a < b ? a : b;
}

float max(float a, float b)
{
    return a > b ? a : b;
}

Color lerp(Color a, Color b, float t)
{
    return {
            max(min(a.r, b.r), min(max(a.r, b.r), (1 - t) * a.r + t * b.r)),
            max(min(a.g, b.g), min(max(a.g, b.g), (1 - t) * a.g + t * b.g)),
            max(min(a.b, b.b), min(max(a.b, b.b), (1 - t) * a.b + t * b.b)),
    };
}

float ilerp(float a, float b, float v)
{
    return (v - a) / (b - a);
}

class ColorGradient
{
public:
    explicit ColorGradient(std::initializer_list<Color> colors...) :
            colors{colors}
    {
        n = this->colors
                .size() - 1;
    }

    Color get_color(float t) const
    {
        unsigned int i = get_index(t);
        float dt = 1.f / n;
        return lerp(colors[i], colors[i + 1], ilerp(i * dt, (i + 1) * dt, t));
    }

private:
    unsigned int get_index(float t) const
    {
        float dt = 1.f / (float) n;
        for (unsigned int i = 0; i < n; i++)
        {
            if (t <= (i + 1) * dt)
            {
                return i;
            }
        }
        return n - 1;
    }

    std::vector<Color> colors;
    unsigned int n;
};

//static ColorGradient colors{
//        Color{0.0, 0.0, 0.0},
//        Color{.14, .14, .77},
//        Color{.46, .96, 1.0},
//        Color{.93, .37, 1.0},
//        Color{1.0, .91, 1.0}
//};
static ColorGradient colors{
        Color{.00, .00, .00},
        Color{.11, .67, .00},
        Color{.61, 1.0, .25},
        Color{1.0, 1.0, .37},
        Color{.93, 1.0, .91}
};

/*
  ----------------------------------------------------------------------
   Update grid with mouse movement
  ----------------------------------------------------------------------
*/

static void get_from_UI(float *d, float *su, float *sv)
{
    int i, j, size = (N + 2) * (N + 2);

    for (i = 0; i < size; i++)
    {
        su[i] = sv[i] = d[i] = 0.0f;
    }

    if (!mouse_down[0] && !mouse_down[2])
    {
        return;
    }

    i = static_cast<int>((static_cast<float>(mx) / static_cast<float>(win_x)) * Nf) + 1;
    j = static_cast<int>((static_cast<float>(my) / static_cast<float>(win_y)) * Nf) + 1;

    if (i < 1 || i > N || j < 1 || j > N)
    {
        return;
    }

    if (mouse_down[0])
    {
        su[IX(i, j)] = force * static_cast<float>(mx - omx);
        sv[IX(i, j)] = force * static_cast<float>(my - omy);
    }

    if (mouse_down[2])
    {
        d[IX(i, j)] = source;
    }

    omx = mx;
    omy = my;
}


void handleEvent(SDL_Event &e)
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

void print_array(float *array)
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
  ----------------------------------------------------------------------
   main
  ----------------------------------------------------------------------
*/

std::vector<float> flat() {
    std::vector<float>data;
    int idx = 0;
    for (auto j = 1; j <= N; j++) {
        for (auto i = 1; i <= N; i++) {
            data.push_back(dens[IX(i, j)]);
            idx++;
        }
    }
    return data;
}

int main(int argc, char **argv)
{
    N = 128;
    Ts = 5;
    Nf = static_cast<float>(N);
    dt = 0.1f;
    diff = 0.0f;
    visc = 0.0001f;
    force = 5.0f;
    source = 100.0f;
    win_x = N * Ts;
    win_y = N * Ts;

    if (!allocate_data())
    {
        exit(1);
    }
    clear_data();

    // Application loop:
    GridWindowManager window(win_x, win_y, N, N);
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
                    print_array(dens);
                }
                if (e.key.keysym.sym == SDLK_c)
                {
                    clear_data();
                }
                if (e.key.keysym.sym == SDLK_v)
                {
                    visc += 0.0001f;
                }
            }
            handleEvent(e);
        }
        get_from_UI(dens_prev, u_prev, v_prev);

        vel_step(N, u, v, u_prev, v_prev, visc, dt);
        dens_step(N, dens, dens_prev, u, v, diff, dt);
        window.display_grid<float>(flat(), [](float d){return colors.get_color(d);});
    }

    free_data();
    return EXIT_SUCCESS;
}