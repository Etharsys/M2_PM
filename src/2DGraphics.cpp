

#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include "2DGraphics.hpp"



GridWindowManager::GridWindowManager(unsigned int window_width, unsigned int window_height,const unsigned int rows, const unsigned int cols, std::string title)
{
    nb_rows = rows;
    
    step_x = 2./rows;
    step_y = 2./cols;
    cell = {
        Position(-1+step_x/2,1-step_y/2),
        Position(-1,1),
        Position(-1+step_x,1),
        Position(-1+step_x,1-step_y),
        Position(-1,1-step_y),
        Position(-1,1)
    };

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

    //Init shaders
    ShadersManager manager;
    manager.load_grid_shaders();
    manager.use_shaders();

    uTranslation = glGetUniformLocation( manager.get_id(), "uTranslation");
    uColor = glGetUniformLocation( manager.get_id(), "uColor");

    //Init vbo
    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
        glBufferData(GL_ARRAY_BUFFER,cell.size()*sizeof(Position),&(cell[0]),GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,0);

    //Init vao
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER,vbo);
            glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
            glVertexAttribPointer(VERTEX_ATTR_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(Position),0);
        glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

}

GridWindowManager::GridWindowManager(unsigned int window_width, unsigned int window_height,const unsigned int rows, const unsigned int cols) :
GridWindowManager(window_width, window_height, rows, cols, "SDLWindow")
{}

GridWindowManager::~GridWindowManager()
{
    glDeleteBuffers(1,&vbo);
    glDeleteVertexArrays(1,&vao);
    SDL_Quit();
}