#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <fstream>
#include <sstream>
#include "ShadersManager.hpp"

/* @brief used to display errors on shaders compilation 
*  Param : GLuint shader : GL int representing a shader
*  Return : string containing compilation error for the shader
*/
std::string Info(GLuint shader)
{
    GLint length;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
	char* log = new char[length];
	glGetShaderInfoLog(shader, length, 0, log);
	std::string logString(log);
	delete [] log;
	return logString;
}

/* @brief used in create shader */
void setSource(const char* src, GLuint id)
{
    glShaderSource(id, 1, &src, 0);
}

/* @brief create a shader using a type and a path
*  Param : GLenum type : type of the shader (usually GL_VERTEX_SHADER OF GL_FRAGMENT_SHADER)
*  Param : std::string path : path to the shader
*  Return : int reprensenting a GL shader
*/
GLuint create_shader(GLenum type, std::string path)
{
    std::ifstream input(path);
    if(!input)
    {
        throw std::runtime_error("Unable to load the shader " + path);
    }

    std::stringstream buffer;
    buffer << input.rdbuf();
    
    GLuint id = glCreateShader(type);
    setSource(buffer.str().c_str(),id);
    return id;
}

/* @brief compile the shader
*  Param : GLuint shader : GL int representing a shader
*  Return : true if the shader got compiled
*/
bool compile_shader(GLuint shader) 
{
	glCompileShader(shader);
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	return status == GL_TRUE;
}

void ShadersManager::use_shaders()
{
    glUseProgram(program_id);
}

void ShadersManager::load_shaders(std::string vs_path, std::string fs_path)
{
    GLuint vs = create_shader(GL_VERTEX_SHADER, vs_path);
    GLuint fs = create_shader(GL_FRAGMENT_SHADER, fs_path);

    if (!compile_shader(vs))
    {
        std::cerr << "Error couldn't compile vs : " << Info(vs) << std::endl;
        exit(EXIT_FAILURE);
    }

    if (!compile_shader(fs))
    {
        std::cerr << "Error couldn't compile fs : " << Info(fs) << std::endl;
        exit(EXIT_FAILURE);
    }

    glAttachShader(program_id, vs);
    glAttachShader(program_id, fs);

    glLinkProgram(program_id);
    GLint status;
	glGetProgramiv(program_id, GL_LINK_STATUS, &status);
	if(status != GL_TRUE)
    {
        std::cerr << "Link Error" << std::endl;
        exit(EXIT_FAILURE);
    };
    glDeleteShader(vs);
    glDeleteShader(fs);
}

void ShadersManager::load_grid_shaders()
{
    load_shaders("shaders/grid.vs.glsl"
        ,"shaders/grid.fs.glsl");
}
