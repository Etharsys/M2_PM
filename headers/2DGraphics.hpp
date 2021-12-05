#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <SDL/SDL.h>

#include "ShadersManager.hpp"
#include "Vertex2D.hpp"
#include "Position.hpp"
#include "Color.hpp"

const GLuint VERTEX_ATTR_POSITION = 0;

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

        /* @brief Retrive non empty grid squares using the function
        *  Param : float step_x : step for x coordinates
        *  Param : float step_y : step for y coordinates
        *  Param : std::vector<T> grid : grid containing datas
        *  Param : Color (*f)(T) : function taking datas from the grid returning a Color containing floats between 0 and 1
        */
        template<typename T>
        std::vector<Vertex2D> get_non_empty_squares(float step_x, float step_y, std::vector<T> grid, Color (*f)(T))
        {
            int rows = 0;
            int cols = 0;
            std::vector<Vertex2D> squares;
            auto it = grid.begin();
            for (auto it = grid.begin(); it != grid.end(); ++it)
            {
                Color c = f(*it);
                if (c.r+c.b+c.g) //if the value is not zero
                {
                    squares.emplace_back(rows*step_x,-cols*step_y,c);
                }

                if ((++rows) >= nb_rows)
                {
                    rows = 0;
                    cols++;
                }
            }
            return squares;
        }
        
        float step_x;
        float step_y;
        unsigned int nb_rows;
        std::vector<Position> cell;

        GLuint vao;
        GLuint vbo;

        GLuint uTranslation;
        GLuint uColor;
};