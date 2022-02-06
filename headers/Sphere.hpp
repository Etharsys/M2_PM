#pragma once

#include <vector>

#include "common.hpp"

class Sphere {

public :
    
    Sphere(GLfloat r, GLsizei discLat, GLsizei discLong);

    ~Sphere();

    // Renvoit le pointeur vers les données
    const ShapeVertex* getDataPointer() const;
    
    // Renvoit le nombre de vertex
    GLsizei getVertexCount() const;

    void draw();

private:
    std::vector<ShapeVertex> m_Vertices;
    GLsizei m_nVertexCount; // Nombre de sommets

    GLuint vao;
    GLuint vbo;
};