#include "Cube.hpp"

Cube::Cube() {
    std::vector<glm::vec3> triangles = {
        glm::vec3(0.5,0.5,0.5), glm::vec3(0.5,-0.5,-0.5),glm::vec3(0.5,-0.5,0.5),
        glm::vec3(-0.5,0.5,-0.5), glm::vec3(0.5,0.5,0.5),glm::vec3(-0.5,0.5,0.5),
        glm::vec3(-0.5,0.5,0.5), glm::vec3(0.5,-0.5,0.5), glm::vec3(-0.5,-0.5,0.5)
    };
    
    for (int n = -1; n < 2; n+=2){
        for (int k = 0; k < 2; k++){
            for(int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++){
                    ShapeVertex vertex;
                    vertex.normal.x = n*(i == 0);
                    vertex.normal.y = n*(i == 1);
                    vertex.normal.z = n*(i == 2);
                    
                    vertex.position = glm::vec3(
                        (1-2*(vertex.normal.x == 0 && k == 0))*triangles[3*i+j][0] - (vertex.normal.x == -1),
                        (1-2*(vertex.normal.y == 0 && k == 0))*triangles[3*i+j][1] - (vertex.normal.y == -1),
                        (1-2*(vertex.normal.z == 0 && k == 0))*triangles[3*i+j][2] - (vertex.normal.z == -1)
                    );
                    if (j == 0) {
                        vertex.texCoords = glm::vec2(1-k,1-k);
                    } else if (j == 1) {
                        vertex.texCoords = glm::vec2(k,k);
                    } else {
                        vertex.texCoords = glm::vec2(1-k,k);
                    }
                    vertices.push_back(vertex);
                }
            }
        }
    }
    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
        glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(ShapeVertex)
            ,&vertices[0],GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,0);

    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER,vbo);
            glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
            glVertexAttribPointer(VERTEX_ATTR_POSITION,3,GL_FLOAT,GL_FALSE,
                sizeof(ShapeVertex), (const GLvoid*) offsetof(ShapeVertex,position));

            glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
            glVertexAttribPointer(VERTEX_ATTR_NORMAL,3,GL_FLOAT,GL_FALSE
                ,sizeof(ShapeVertex), (const GLvoid*) offsetof(ShapeVertex,normal));
            
            glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);
            glVertexAttribPointer(VERTEX_ATTR_TEXTURE,2,GL_FLOAT,GL_FALSE
                ,sizeof(ShapeVertex), (const GLvoid*) offsetof(ShapeVertex,texCoords));
        glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
}

Cube::~Cube()  {
    glDeleteBuffers(1,&vbo);
    glDeleteVertexArrays(1,&vao);
}

void Cube::draw()  {
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glBindVertexArray(0);
}

const ShapeVertex* Cube::getDataPointer() const {
    return &vertices[0];
}

GLsizei Cube::getVertexCount() const {
    return vertices.size();
}