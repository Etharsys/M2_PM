#include <ColorGradient.hpp>


Color lerp(Color a, Color b, float t)
{
    using namespace std;
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

ColorGradient::ColorGradient(std::initializer_list<Color> colors, ...) :
        colors{colors}
{
    n = this->colors.size() - 1;
}

Color ColorGradient::get_color(float t) const
{
    unsigned int i = get_index(t);
    auto fi = static_cast<float>(i);
    float dt = 1.f / static_cast<float>(n);
    return lerp(colors[i], colors[i + 1], ilerp(fi * dt, (fi + 1) * dt, t));
}

unsigned int ColorGradient::get_index(float t) const
{
    float dt = 1.f / (float) n;
    for (unsigned int i = 0; i < n; i++)
    {
        if (t <= static_cast<float>(i + 1) * dt)
        {
            return i;
        }
    }
    return n - 1;
}
