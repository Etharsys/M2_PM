#pragma once

#include <vector>
#include <iostream>
#include <functional>

#include "glm.hpp"

#include "Camera.hpp"
#include "ShadersManager.hpp"
#include "Color.hpp"
#include "Sphere.hpp"
#include "Cube.hpp"


/* @brief class used to manage and display windows
*  Can currently only display grids.
*/
class CubeWindowManager
{  
    public :
        /* @brief Create a CubeWindowManager used to display sphere given a boundarie box
        *  Param : width : width of the window
        *  Param : height : height of the window
        *  Param : size_y : the height of the box
        *  Param : size_x : the width of the box
        *  Param : size_z : the depth of the box
        *  Param : light : if you want to load shaders with lights
        *  Param : title : the title of the window
        */
        CubeWindowManager(unsigned int width, unsigned int height
            , const unsigned int box_height, const unsigned int box_width, const unsigned int box_depth);
        CubeWindowManager(unsigned int width, unsigned int height
            , const unsigned int box_height, const unsigned int box_width, const unsigned int box_depth, bool light);
        CubeWindowManager(unsigned int width, unsigned int height
            , const unsigned int box_height, const unsigned int box_width, const unsigned int box_depth, bool light, std::string title);
        ~CubeWindowManager();

        /* @brief Display the spheres in white in the given box (coordinates must be positive)
        *  Param : grid : vector representing the grid
        *  Param : get_color : function taking elements from the vector grid returning a Color containing float between 0 and 1 
        *  Param : get_pos : function taking an element returning its position
        */
        template <typename T>
        void display_cubes(const std::vector<float>& cubes, int X, int Y, int Z, float max) const
        {
            glClearColor(0.1f,0.1f,0.1f,1.f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glm::mat4 globalMVMatrix = camera.getViewMatrix();
            glm::vec4 light_dir = globalMVMatrix*glm::vec4(1.,1.,1.,0.);

            glUniform3f(uLightDir_vs, light_dir.x, light_dir.y, light_dir.z);
            glUniform3f(uLightIntensity, 1., 1., 1.);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            float Localvalue = 0.;
            for (int z = 0; z < Z; z++){
                for (int y = 0; y < Y; y++){
                    for (int x = 0; x < X; x++){
                        Localvalue = cubes.at(x + (y*X) + (z*X*Y));
                        if(Localvalue > 0 && Localvalue/max > 0.0001 ){
                            glm::vec3 pos = glm::vec3(x,y,z);
                            glm::mat4 cubesMatrix = glm::translate(globalMVMatrix, pos);

                            glUniformMatrix4fv(uMVMatrix, 1, false, glm::value_ptr(cubesMatrix));
                            glUniformMatrix4fv(uMVPMatrix, 1, false, glm::value_ptr(projMatrix * cubesMatrix));
                            glUniformMatrix4fv(uNormalMatrix, 1, false, glm::value_ptr(glm::transpose(glm::inverse(cubesMatrix))));
                            glUniform4f(uColor, 1., 1., 1., Localvalue/max);
                            unit_cube->draw();
                        }
                    }
                }
            }
            //draw bound box
            glm::mat4 bounboxMatrix = glm::scale(glm::translate(globalMVMatrix, boundbox_trans), boundbox_scale);
            glUniformMatrix4fv(uMVMatrix, 1, false, glm::value_ptr(bounboxMatrix));
            glUniformMatrix4fv(uMVPMatrix, 1, false, glm::value_ptr(projMatrix * bounboxMatrix));
            glUniformMatrix4fv(uNormalMatrix, 1, false, glm::value_ptr(glm::transpose(glm::inverse(bounboxMatrix))));
            glUniform4f(uColor, 1., 1., 1., 1.);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            boundbox->draw();
            SDL_GL_SwapBuffers();
        }

        Camera camera;

    private :
        Cube* unit_cube;
        Cube* boundbox;
        glm::vec3 boundbox_trans;
        glm::vec3 boundbox_scale;

        glm::mat4 projMatrix;

        GLuint uMVMatrix;
        GLuint uMVPMatrix;
        GLuint uNormalMatrix;

        GLuint uLightDir_vs;
        GLuint uLightIntensity;
        
        GLuint uColor;
};