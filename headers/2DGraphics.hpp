#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <SDL/SDL.h>

#include "ShadersManager.hpp"

const GLuint VERTEX_ATTR_POSITION = 0;

/*Structure for positions*/
struct Position
{
    Position() : x(0), y(0) {}
    Position(float _x,float _y) : x(_x), y(_y) {}

    float x;
    float y;
};

/*Structure for colors*/
struct Color
{
    Color() : r(0), g(0), b(0){}
    Color(float _r, float _g, float _b) : r(_r), g(_g), b(_b){}

    float r;
    float g;
    float b;
};

/*@brief used to display the grid*/
class Vertex2D
{
    public:
        Vertex2D(float x, float y, float r, float g, float b) : pos(x,y), col(r,g,b){};
        Vertex2D(float x, float y, Color c) : pos(x,y), col(c){};
        Position pos;
        Color col;
};

class Cell2D
{
    public :
        Cell2D(Position _a, Position _b, Position _c, Position _d, Position _e, Position _f) :
        a(_a), b(_b), c(_c), d(_d), e(_e), f(_f){};
    
        Position a,b,c,d,e,f;
};

/* @brief Retrive non empty grid squares using the function
*  Param : float step_x : step for x coordinates
*  Param : float step_y : step for y coordinates
*  Param : std::vector<T> grid : grid containing datas
*  Param : Color (*f)(T) : function taking datas from the grid returning a Color having values between 0 and 1
*/
template<typename T>
std::vector<Vertex2D> get_non_empty_squares(float step_x, float step_y, std::vector<T> grid, Color (*f)(T))
{
    float i = -1;
    float j = 1;

    std::vector<Vertex2D> squares;
    auto it = grid.begin();
    while (it != grid.end())
    {
        Color c = f(*it);
        if (c.r+c.b+c.g) //if the value is not zero
        {
            squares.emplace_back(i+1,j-1,c);
        }

        ++it;
        if ((i = i+step_x) >= 1)
        {
            i = -1;
            j -= step_y;
        }
    }
    return squares;
}

/* @brief class used to manage and display windows
*  Can currently only display grids.
*/
class SDLWindowManager
{  
    public :

        SDLWindowManager(unsigned int width, unsigned int height);
        SDLWindowManager(unsigned int width, unsigned int height, std::string title);
        ~SDLWindowManager();

        /* @brief Display the 2D grid in black and white using the number of rows, columns and a function
        *  Param : rows : number of rows
        *  Param : cols : number of columns
        *  Param : grid : vector representing the grid
        *  Param : f    : function taking elements from the vector grid returning an float between 0 and 1 
        * (may be identity) (representing the density of the fluid on a cell)
        *  !!!!(f can return either 0 or 1 at this point)!!!!
        */
        template <typename T>
        void display_grid(const unsigned int rows, const unsigned int cols, const std::vector<T>& grid, Color (*get_color)(T))
        {
            
            GLenum glewInitError = glewInit();
            if(GLEW_OK != glewInitError) {
                std::cerr << glewGetErrorString(glewInitError) << std::endl;
                return;
            }

            //Init shaders
            ShadersManager manager;
            manager.load_grid_shaders();
            manager.use_shaders();

            float step_x = 2./rows;
            float step_y = 2./cols;

            std::vector<Position> first_cell = 
                   {Position(-1+step_x/2,1-step_y/2),
                    Position(-1,1),
                    Position(-1+step_x,1),
                    Position(-1+step_x,1-step_y),
                    Position(-1,1-step_y),
                    Position(-1,1)};

            //Init vbo
            GLuint vbo;

            glGenBuffers(1,&vbo);
            glBindBuffer(GL_ARRAY_BUFFER,vbo);
                glBufferData(GL_ARRAY_BUFFER,first_cell.size()*sizeof(Position),&(first_cell[0]),GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER,0);

            //Init vao
            GLuint vao;
            glGenVertexArrays(1,&vao);
            glBindVertexArray(vao);
                glBindBuffer(GL_ARRAY_BUFFER,vbo);
                    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
                    glVertexAttribPointer(VERTEX_ATTR_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(Position),0);
                glBindBuffer(GL_ARRAY_BUFFER,0);
            glBindVertexArray(0);
            
            GLint uTranslation = glGetUniformLocation( manager.get_id(), "uTranslation");
            GLint uColor = glGetUniformLocation( manager.get_id(), "uColor");

            bool done = false;
            while(!done)
            {
                std::vector<Vertex2D> squares = get_non_empty_squares(step_x, step_y, grid, get_color);
                glClear(GL_COLOR_BUFFER_BIT);
                SDL_Event e;
                while(SDL_PollEvent(&e))
                {
                    if (e.type == SDL_QUIT)
                    {
                        done = true;
                    }
                }

                glBindVertexArray(vao);
                for (auto vertex : squares)
                {
                    glUniform2f(uTranslation, vertex.pos.x, vertex.pos.y);
                    glUniform3f(uColor, vertex.col.r, vertex.col.g, vertex.col.b);
                    glDrawArrays(GL_TRIANGLE_FAN,0,first_cell.size());
                }
                glBindVertexArray(0);
                SDL_GL_SwapBuffers();
            }

            glDeleteBuffers(1,&vbo);
            glDeleteVertexArrays(1,&vao);
        }
};