#include <fluid2d/Data.hpp>

#include <exception>
#include <cstdlib>

#define IX(i, j) ((i)+(N+2)*(j))

fluid2d::Data::Data(int N) : N{N}
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

fluid2d::Data::~Data()
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

void fluid2d::Data::clear() const
{
    int i, size = (N + 2) * (N + 2);

    for (i = 0; i < size; i++)
    {
        u[i] = v[i] = u_prev[i] = v_prev[i] = dens[i] = dens_prev[i] = 0.0f;
    }
}

std::vector<float> fluid2d::Data::flat() const
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

void fluid2d::Data::get_from_UI(fluid2d::Event &event, const fluid2d::Config &config) const
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
