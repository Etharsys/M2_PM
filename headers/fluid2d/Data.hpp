#pragma once

#include <vector>

#include <fluid2d/Event.hpp>
#include <fluid2d/Config.hpp>

namespace fluid2d
{
    struct Data
    {
        explicit Data(int N);

        ~Data();

        void clear() const;

        std::vector<float> flat() const;

        void get_from_UI(Event &event, const Config &config) const;

        void dens_step(const Config& config);

        void vel_step(const Config& config);

        int N;
        float *u, *v, *u_prev, *v_prev;
        float *dens, *dens_prev;
    };
}