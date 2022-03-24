#include "CubeWindowManager.hpp"

#include <iostream>
#include "glm.hpp"

CubeWindowManager::CubeWindowManager(unsigned int window_width, unsigned int window_height
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

    unit_cube = new Cube();
    boundbox = new Cube();
    boundbox_trans = glm::vec3(box_width/2., box_height/2., box_depth/2.);
    boundbox_scale = glm::vec3(box_width, box_height, box_depth);
    projMatrix = glm::perspective(glm::radians(70.f), window_width/(float)window_height, 0.1f, 1000.f);

    //Init shaders
    ShadersManager manager;
    if (light) {
        manager.load_cube_shaders();
    } else {
        manager.load_cube_shaders();
    }
    manager.use_shaders();

    uLightDir_vs = glGetUniformLocation( manager.get_id(), "uLightDir_vs");
    uLightIntensity = glGetUniformLocation( manager.get_id(), "uLightIntensity");
    
    uColor = glGetUniformLocation( manager.get_id(), "uColor");
    uMVMatrix = glGetUniformLocation( manager.get_id(), "uMVMatrix");
    uMVPMatrix = glGetUniformLocation( manager.get_id(), "uMVPMatrix");
    uNormalMatrix = glGetUniformLocation( manager.get_id(), "uNormalMatrix");
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

CubeWindowManager::CubeWindowManager(unsigned int window_width, unsigned int window_height
            , const unsigned int box_height, const unsigned int box_width, const unsigned int box_depth) :
CubeWindowManager(window_width, window_height, box_height, box_width, box_depth, false, "SDLWindow")
{}

CubeWindowManager::CubeWindowManager(unsigned int window_width, unsigned int window_height
            , const unsigned int box_height, const unsigned int box_width, const unsigned int box_depth, bool light) :
CubeWindowManager(window_width, window_height, box_height, box_width, box_depth, light, "SDLWindow")
{}

CubeWindowManager::~CubeWindowManager()
{
    delete unit_cube;
    delete boundbox;
    SDL_Quit();
}