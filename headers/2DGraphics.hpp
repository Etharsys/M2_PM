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
*  Param : Color (*f)(T) : function taking datas from the grid returning a Color containing floats between 0 and 1
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
class GridWindowManager
{  
    public :
        /* @brief Create a GridWindowManager used to display the given grid
        *  Param : width : width of the window
        *  Param : height : height of the window
        *  Param : rows : number of rows
        *  Param : cols : number of columns
        */
        GridWindowManager(unsigned int width, unsigned int height,const unsigned int rows, const unsigned int cols);
        GridWindowManager(unsigned int width, unsigned int height,const unsigned int rows, const unsigned int cols, std::string title);
        ~GridWindowManager();

        /* @brief Display the 2D grid in black and white using the number of rows, columns and a function
        *  Param : grid : vector representing the grid
        *  Param : f    : function taking elements from the vector grid returning a Color containing float between 0 and 1 
        * (may be identity (id,id,id) for shades of grey) (representing the density of the fluid on a cell)
        */
        template <typename T>
        void display_grid(const std::vector<T>& grid, Color (*get_color)(T))
        {
            std::vector<Vertex2D> squares = get_non_empty_squares(step_x, step_y, grid, get_color);
            glClear(GL_COLOR_BUFFER_BIT);

            glBindVertexArray(vao);
            for (auto vertex : squares)
            {
                glUniform2f(uTranslation, vertex.pos.x, vertex.pos.y);
                glUniform3f(uColor, vertex.col.r, vertex.col.g, vertex.col.b);
                glDrawArrays(GL_TRIANGLE_FAN,0,cell.size());
            }
            glBindVertexArray(0);
            SDL_GL_SwapBuffers();
        }
    private : 
        float step_x;
        float step_y;
        std::vector<Position> cell;

        GLuint vao;
        GLuint vbo;

        GLuint uTranslation;
        GLuint uColor;
};