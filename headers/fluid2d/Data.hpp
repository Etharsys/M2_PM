#pragma once

#include <vector>

#include <fluid2d/Event.hpp>

namespace fluid2d
{
    struct Data
    {
        explicit Data(int N, float dt, float diff, float visc, float source);

        ~Data();

        void clear() const;

        std::vector<float> flat() const;

        void get_from_UI(Event &event) const;

        void dens_step();

        void vel_step();

        void update_viscosity(float x) {
            visc += x;
        }

    private:
        int N;
        float dt, diff, visc, source;
        float *u, *v, *u_prev, *v_prev;
        float *dens, *dens_prev;

        int size() const;
    };
}