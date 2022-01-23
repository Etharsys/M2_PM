#pragma once
#include <string>
#include <GL/glew.h>
#include <SDL/SDL.h>

/* @brief class used to manage shaders*/
class ShadersManager
{   
    public :
        ShadersManager() : program_id(glCreateProgram()){};

        ~ShadersManager()
        {
            glDeleteProgram(program_id);
        }

        GLuint get_id() { return program_id; }

        /* @brief Load grid.vs.glsl and grid.fs.glsl shaders*/
        void load_grid_shaders();

        /* @brief Load shaders vs_path and fs_path (from the folder M2_PM/build)
        *  Param : std::string vs_path : path for vertex shader
        *  Param : std::string fs_path : path for fragment shader
        *  Exit with EXIT_FAILURE if vs_path of fs_path got compilation errors
        */
        void load_shaders(std::string vs_path, std::string fs_path);

        /* @brief use shaders previously compiled*/
        void use_shaders();

    private : 
        GLuint program_id;
};