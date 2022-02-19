#include "SphereWindowManager.hpp"

#include <iostream>
#include "glm.hpp"

SphereWindowManager::SphereWindowManager(unsigned int window_width, unsigned int window_height
    , const unsigned int box_height, const unsigned int box_width, const unsigned int box_depth, bool light,  std::string title) :
        camera(glm::vec3(box_width/2., box_height/2., box_depth/2.))
{
    if (0 != SDL_Init( SDL_INIT_EVERYTHING ))
    {
        std::cerr << SDL_GetError() << std::endl;
        return;
    }
    if(!SDL_SetVideoMode(window_width, window_height, 32, SDL_OPENGL)) 
    {
        std::cerr << SDL_GetError() << std::endl;
        return;
    }
    SDL_WM_SetCaption("Grid", nullptr);

    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) 
    {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return;
    }

    unit_sphere = new Sphere(0.5, 32, 32);
    boundbox = new Cube();
    boundbox_trans = glm::vec3(box_width/2., box_height/2., box_depth/2.);
    boundbox_scale = glm::vec3(box_width, box_height, box_depth);
    projMatrix = glm::perspective(glm::radians(70.f), window_width/(float)window_height, 0.1f, 1000.f);

    //Init shaders
    ShadersManager manager;
    if (light) {
        manager.load_sphere_blinphong_shaders();
    } else {
        manager.load_sphere_shaders();
    }
    manager.use_shaders();

    uLightDir_vs = glGetUniformLocation( manager.get_id(), "uLightDir_vs");
    uLightIntensity = glGetUniformLocation( manager.get_id(), "uLightIntensity");
    
    uColor = glGetUniformLocation( manager.get_id(), "uColor");
    uMVMatrix = glGetUniformLocation( manager.get_id(), "uMVMatrix");
    uMVPMatrix = glGetUniformLocation( manager.get_id(), "uMVPMatrix");
    uNormalMatrix = glGetUniformLocation( manager.get_id(), "uNormalMatrix");
    glEnable(GL_DEPTH_TEST);
}

SphereWindowManager::SphereWindowManager(unsigned int window_width, unsigned int window_height
            , const unsigned int box_height, const unsigned int box_width, const unsigned int box_depth) :
SphereWindowManager(window_width, window_height, box_height, box_width, box_depth, false, "SDLWindow")
{}

SphereWindowManager::SphereWindowManager(unsigned int window_width, unsigned int window_height
            , const unsigned int box_height, const unsigned int box_width, const unsigned int box_depth, bool light) :
SphereWindowManager(window_width, window_height, box_height, box_width, box_depth, light, "SDLWindow")
{}

SphereWindowManager::~SphereWindowManager()
{
    delete unit_sphere;
    delete boundbox;
    SDL_Quit();
}