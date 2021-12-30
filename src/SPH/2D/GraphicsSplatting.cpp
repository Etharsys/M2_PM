#include <Graphics/2DGraphics.hpp>
#include <SPH/2D/Fluid2D.hpp>

int main(int argc, char const *argv[])
{

    Fluid fluid{1000};
    int size = 1000;
    GridWindowManager splatting_window(fluid.WIDTH, 50,size,1,"SPH splatting method");
    bool done = false;
    while (!done)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                done = true;
            }
        }
        splatting_window.display_grid<float>(fluid.render_top_surface(size),
                                        [](float value)
                                        {
                                            return Color(0, 0, value);
                                        });
        fluid.update();
    }
    return 0;
}