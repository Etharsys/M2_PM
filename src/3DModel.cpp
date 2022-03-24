// Standard
#include <cstdlib>

// Project
#include <Color.hpp>
#include <Cube.hpp>
#include <ColorGradient.hpp>
#include <fluid3d/Data.hpp>
#include <CubeWindowManager.hpp>
#include <UserInteraction.hpp>

#include <vector>
#include <glm.hpp>



#define DEBUG true

#define N 15
#define MAX_DENSITY 10



int main()
{
    // Structure init
    fluid3d::FluidData data(N, 0., .0001, .1);
    CubeWindowManager windowManager(800,800, N, N, N, false);
    UserInteractions userInteractions;

    // init sources for application
    data.FluidDataAddDensity(N/2, N/2, N/2, 10.);
    data.FluidDataAddVelocity(N/2, N/2, N/2, 0, -10, 0);
    
    // display init settings
    windowManager.display_cubes<glm::vec3>(data.flat(), N, N, N, MAX_DENSITY);
    
    // Simulation loop
    while (!userInteractions.isdone()) {

        userInteractions.getUserInteraction();
        userInteractions.applyUserInteraction(windowManager.camera);

        if (!DEBUG || userInteractions.stepDebug())
        {
            data.FluidDataStep();
        }

        windowManager.display_cubes<glm::vec3>(data.flat(), N, N, N, MAX_DENSITY);
    }

    return EXIT_SUCCESS;
}
