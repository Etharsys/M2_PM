#pragma once

namespace fluid2d
{
    struct Config
    {
        Config(int n, int ts, float dt, float diff, float visc, float force, float source);

        int size() const;

        int N;
        int Ts;
        float Nf;
        float dt, diff, visc;
        float force, source;
    };
}