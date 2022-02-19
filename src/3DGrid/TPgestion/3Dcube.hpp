#include <glimac/SDLWindowManager.hpp>
#include <glimac/Sphere.hpp>
#include <glimac/Cube.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/Program.hpp> 
#include <glimac/FilePath.hpp>
#include <glm/gtc/random.hpp>
#include <glimac/Image.hpp>
#include <glimac/TrackballCamera.hpp>

using namespace glimac;

struct CubeProgram {
    Program m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;

    GLint uColor;

    CubeProgram(const FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "shaders/spheres.vs.glsl",
                              applicationPath.dirPath() + "shaders/spheres.fs.glsl")) {
        uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
        uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
        uColor = glGetUniformLocation(m_Program.getGLId(), "uColor");
    }
};


int display(char* path, int width, int height, int depth, std::vector<float> to_display);
