#include "Sphere.hpp"

Sphere::Sphere(GLfloat r, GLsizei discLat, GLsizei discLong){
    GLfloat rcpLat = 1.f / discLat, rcpLong = 1.f / discLong;
    GLfloat dPhi = 2 * glm::pi<float>() * rcpLat, dTheta = glm::pi<float>() * rcpLong;
    
    std::vector<ShapeVertex> data;
    
    for(GLsizei j = 0; j <= discLong; ++j) {
        GLfloat cosTheta = cos(-glm::pi<float>() / 2 + j * dTheta);
        GLfloat sinTheta = sin(-glm::pi<float>() / 2 + j * dTheta);
        
        for(GLsizei i = 0; i <= discLat; ++i) {
            ShapeVertex vertex;
            
            vertex.texCoords.x = i * rcpLat;
            vertex.texCoords.y = 1.f - j * rcpLong;

            vertex.normal.x = sin(i * dPhi) * cosTheta;
            vertex.normal.y = sinTheta;
            vertex.normal.z = cos(i * dPhi) * cosTheta;
            
            vertex.position = r * vertex.normal;
            
            data.push_back(vertex);
        }
    }

    m_nVertexCount = discLat * discLong * 6;
    
    GLuint idx = 0;
    for(GLsizei j = 0; j < discLong; ++j) {
        GLsizei offset = j * (discLat + 1);
        for(GLsizei i = 0; i < discLat; ++i) {
            m_Vertices.push_back(data[offset + i]);
            m_Vertices.push_back(data[offset + (i + 1)]);
            m_Vertices.push_back(data[offset + discLat + 1 + (i + 1)]);
            m_Vertices.push_back(data[offset + i]);
            m_Vertices.push_back(data[offset + discLat + 1 + (i + 1)]);
            m_Vertices.push_back(data[offset + i + discLat + 1]);
        }
    }

    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
        glBufferData(GL_ARRAY_BUFFER,m_Vertices.size()*sizeof(ShapeVertex)
            ,&m_Vertices[0],GL_STATIC_DRAW);
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

Sphere::~Sphere() {
    glDeleteBuffers(1,&vbo);
    glDeleteVertexArrays(1,&vao);
}

const ShapeVertex* Sphere::getDataPointer() const {
    return &m_Vertices[0];
}

GLsizei Sphere::getVertexCount() const {
    return m_nVertexCount;
}

void Sphere::draw() {
    glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES,0, m_nVertexCount);
    glBindVertexArray(0);
}