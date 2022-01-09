#include <fluid2d/Config.hpp>

namespace fluid2d
{
    Config::Config(int n, int ts, float dt, float diff, float visc, float force, float source) :
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

    int Config::size() const
    {
        return (N + 2) * (N + 2);
    }
}