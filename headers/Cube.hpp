#pragma once

#include <iostream>
#include <vector>

#include "common.hpp"

class Cube {

    public:
        Cube();

        ~Cube();

        void draw();

        const ShapeVertex* getDataPointer() const;

        GLsizei getVertexCount() const;
    private:
        std::vector<ShapeVertex> vertices;
        GLuint vao;
        GLuint vbo;
};