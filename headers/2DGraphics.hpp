 #pragma once
 #include <iostream>
 #include <string>
 #include <vector>
 #include <GL/glew.h>
 #include <SDL/SDL.h>
 

const GLuint VERTEX_ATTR_POSITION = 0;

struct Position
{
    Position(float _x,float _y) : x(_x), y(_y){};

    float x;
    float y;
};

/*@brief used to display the grid*/
class Vertex2D
{
    public:
        Vertex2D(float x, float y) : pos(x,y){};

        Position pos;
};

template<typename T>
std::vector<Vertex2D> get_non_empty_squares(float step_x, float step_y, std::vector<T> grid, float (*f)(T))
{
    float i = -1;
    float j = 1;

    std::vector<Vertex2D> squares;
    auto it = grid.begin();
    while (it != grid.end())
    {
        if (f(*it)) //if the value is not zero
        {
            //f(*it) will be use later to display density of dye
            squares.emplace_back(i+step_x/2,j-step_y/2);
            squares.emplace_back(i,j);
            squares.emplace_back(i+step_x,j);
            squares.emplace_back(i+step_x,j-step_y);
            squares.emplace_back(i,j-step_y);
            squares.emplace_back(i,j);
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

class SDLWindowManager
{  
    public :

        SDLWindowManager(unsigned int width, unsigned int height);
        SDLWindowManager(unsigned int width, unsigned int height, std::string title);
        ~SDLWindowManager();

        /* @brief Display the 2D grid using the number of rows, columns and a function
        *  Param : rows : number of rows
        *  Param : cols : number of columns
        *  Param : grid : vector representing the grid
        *  Param : f    : function taking elements from the vector grid returning an float between 0 and 1 
        * (may be identity) (representing the density of the fluid on a cell)
        *  !!!!(f can return either 0 or 1 at this point)!!!!
        */
        template <typename T>
        void display_grid(const unsigned int rows, const unsigned int cols, const std::vector<T>& grid, float (*f)(T))
        {
            std::vector<Vertex2D> squares = get_non_empty_squares(2./rows, 2./cols, grid, f);
            
            GLenum glewInitError = glewInit();
            if(GLEW_OK != glewInitError) {
                std::cerr << glewGetErrorString(glewInitError) << std::endl;
                return;
            }

            //Init vbo
            GLuint vbo;

            glGenBuffers(1,&vbo);
            glBindBuffer(GL_ARRAY_BUFFER,vbo);
                glBufferData(GL_ARRAY_BUFFER,squares.size()*sizeof(Vertex2D),&(squares[0]),GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER,0);

            //Init vao
            GLuint vao;
            glGenVertexArrays(1,&vao);
            glBindVertexArray(vao);
                glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
                glBindBuffer(GL_ARRAY_BUFFER,vbo);
                    glVertexAttribPointer(VERTEX_ATTR_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D),(const GLvoid*) offsetof(Vertex2D,pos));
                glBindBuffer(GL_ARRAY_BUFFER,0);
            glBindVertexArray(0);

            bool done = false;
            while(!done)
            {
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
                for (int i = 0; i < squares.size(); i += 6)
                {
                    glDrawArrays(GL_TRIANGLE_FAN,i,6);
                }
                glBindVertexArray(0);
                SDL_GL_SwapBuffers();
            }

            glDeleteBuffers(1,&vbo);
            glDeleteVertexArrays(1,&vao);
        }
};