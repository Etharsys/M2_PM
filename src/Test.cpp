#include <vector>
#include <iostream>
#include "glm.hpp"
#include "SphereWindowManager.hpp"
#include "UserInteraction.hpp"

int main() {
    SphereWindowManager windowManager(800,600, 20, 20, 20);
    UserInteractions userInteractions;
    std::vector<glm::vec3> spheres = {glm::vec3(10,10,10), glm::vec3(10,7,10), glm::vec3(10,10,5)};

    while (!userInteractions.isdone()) {

        userInteractions.getUserInteraction();

        userInteractions.applyUserInteraction(windowManager.camera);

        windowManager.display_spheres<glm::vec3>(spheres, [](glm::vec3 a) { return Color(1,0,1);}, [](glm::vec3 a) {return a;});
    }

    return 0;
}
