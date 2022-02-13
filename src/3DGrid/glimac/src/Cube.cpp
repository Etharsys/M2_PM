#include <cmath>
#include <vector>
#include <iostream>
#include "glimac/common.hpp"
#include "glimac/Cube.hpp"

namespace glimac {

void Cube::build(GLfloat r) {
    std::vector<ShapeVertex> data;
            
    // Construit l'ensemble des vertex
            ShapeVertex vertex;
//1
            vertex.texCoords.x = 1;
            vertex.texCoords.y = 0.;

            vertex.normal.x = 1;
            vertex.normal.y = 1;
            vertex.normal.z = 1;

            vertex.position.x = vertex.normal.x*r/2;
            vertex.position.y = vertex.normal.y*r/2;
            vertex.position.z = vertex.normal.z*r/2;

            data.push_back(vertex);
//2
            vertex.texCoords.x = 1;
            vertex.texCoords.y = 1.;

            vertex.normal.x = 1;
            vertex.normal.y = -1;
            vertex.normal.z = 1;
            
            vertex.position.x = vertex.normal.x*r/2;
            vertex.position.y = vertex.normal.y*r/2;
            vertex.position.z = vertex.normal.z*r/2;
            data.push_back(vertex);
//3
            vertex.texCoords.x = 0;
            vertex.texCoords.y = 0.;

            vertex.normal.x = -1;
            vertex.normal.y = 1;
            vertex.normal.z = 1;

            vertex.position.x = vertex.normal.x*r/2;
            vertex.position.y = vertex.normal.y*r/2;
            vertex.position.z = vertex.normal.z*r/2;
            data.push_back(vertex);
//4         
            vertex.texCoords.x = 0;
            vertex.texCoords.y = 0.;
            // |/

            vertex.normal.x = 1;
            vertex.normal.y = 1;
            vertex.normal.z = 1;

            vertex.position.x = vertex.normal.x*r/2;
            vertex.position.y = vertex.normal.y*r/2;
            vertex.position.z = vertex.normal.z*r/2;
            data.push_back(vertex);
//5         
            vertex.texCoords.x = 0;
            vertex.texCoords.y = 1.;

            vertex.normal.x = 1;
            vertex.normal.y = -1;
            vertex.normal.z = 1;

            vertex.position.x = vertex.normal.x*r/2;
            vertex.position.y = vertex.normal.y*r/2;
            vertex.position.z = vertex.normal.z*r/2;
            data.push_back(vertex);
//6         
            vertex.texCoords.x = 1;
            vertex.texCoords.y = 1.;

            vertex.normal.x = 1;
            vertex.normal.y = 1;
            vertex.normal.z = -1;

            vertex.position.x = vertex.normal.x*r/2;
            vertex.position.y = vertex.normal.y*r/2;
            vertex.position.z = vertex.normal.z*r/2;
            data.push_back(vertex);
//7         
            vertex.texCoords.x = 1;
            vertex.texCoords.y = 1.;
            // \|

            vertex.normal.x = 1;
            vertex.normal.y = 1;
            vertex.normal.z = 1;

            vertex.position.x = vertex.normal.x*r/2;
            vertex.position.y = vertex.normal.y*r/2;
            vertex.position.z = vertex.normal.z*r/2;
            data.push_back(vertex);
//8          
            vertex.texCoords.x = 0;
            vertex.texCoords.y = 1.;

            vertex.normal.x = -1;
            vertex.normal.y = 1;
            vertex.normal.z = 1;

            vertex.position.x = vertex.normal.x*r/2;
            vertex.position.y = vertex.normal.y*r/2;
            vertex.position.z = vertex.normal.z*r/2;
            data.push_back(vertex);
//9         
            vertex.texCoords.x = 1;
            vertex.texCoords.y = 0.;

            vertex.normal.x = 1;
            vertex.normal.y = 1;
            vertex.normal.z = -1;

            vertex.position.x = vertex.normal.x*r/2;
            vertex.position.y = vertex.normal.y*r/2;
            vertex.position.z = vertex.normal.z*r/2;
            data.push_back(vertex);//ok
//10         
            vertex.texCoords.x = 0;
            vertex.texCoords.y = 0.;

            vertex.normal.x = -1;
            vertex.normal.y = 1;
            vertex.normal.z = -1;

            vertex.position.x = vertex.normal.x*r/2;
            vertex.position.y = vertex.normal.y*r/2;
            vertex.position.z = vertex.normal.z*r/2;
            data.push_back(vertex);
//11
            vertex.texCoords.x = 0;
            vertex.texCoords.y = 1.;

            vertex.normal.x = -1;
            vertex.normal.y = 1;
            vertex.normal.z = 1;

            vertex.position.x = vertex.normal.x*r/2;
            vertex.position.y = vertex.normal.y*r/2;
            vertex.position.z = vertex.normal.z*r/2;
            data.push_back(vertex);
//12        
            vertex.texCoords.x = 1;
            vertex.texCoords.y = 0.;

            vertex.normal.x = 1;
            vertex.normal.y = 1;
            vertex.normal.z = -1;

            vertex.position.x = vertex.normal.x*r/2;
            vertex.position.y = vertex.normal.y*r/2;
            vertex.position.z = vertex.normal.z*r/2;
            data.push_back(vertex);
//13        
            vertex.texCoords.x = 1;
            vertex.texCoords.y = 0.;
            //111

            vertex.normal.x = -1;
            vertex.normal.y = 1;
            vertex.normal.z = -1;

            vertex.position.x = vertex.normal.x*r/2;
            vertex.position.y = vertex.normal.y*r/2;
            vertex.position.z = vertex.normal.z*r/2;
            data.push_back(vertex);
//14        
            vertex.texCoords.x = 1;
            vertex.texCoords.y = 1.;

            vertex.normal.x = -1;
            vertex.normal.y = -1;
            vertex.normal.z = -1;

            vertex.position.x = vertex.normal.x*r/2;
            vertex.position.y = vertex.normal.y*r/2;
            vertex.position.z = vertex.normal.z*r/2;
            data.push_back(vertex);
//15        
            vertex.texCoords.x = 0;
            vertex.texCoords.y = 0.;

            vertex.normal.x = 1;
            vertex.normal.y = 1;
            vertex.normal.z = -1;

            vertex.position.x = vertex.normal.x*r/2;
            vertex.position.y = vertex.normal.y*r/2;
            vertex.position.z = vertex.normal.z*r/2;
            data.push_back(vertex);
//16        
            vertex.texCoords.x = 0;
            vertex.texCoords.y = 0.;
            //--1

            vertex.normal.x = -1;
            vertex.normal.y = 1;
            vertex.normal.z = -1;

            vertex.position.x = vertex.normal.x*r/2;
            vertex.position.y = vertex.normal.y*r/2;
            vertex.position.z = vertex.normal.z*r/2;
            data.push_back(vertex);
//17        
            vertex.texCoords.x = 0;
            vertex.texCoords.y = 1.;

            vertex.normal.x = -1;
            vertex.normal.y = -1;
            vertex.normal.z = -1;

            vertex.position.x = vertex.normal.x*r/2;
            vertex.position.y = vertex.normal.y*r/2;
            vertex.position.z = vertex.normal.z*r/2;
            data.push_back(vertex);
//18        
            vertex.texCoords.x = 1;
            vertex.texCoords.y = 0.;

            vertex.normal.x = -1;
            vertex.normal.y = 1;
            vertex.normal.z = 1;

            vertex.position.x = vertex.normal.x*r/2;
            vertex.position.y = vertex.normal.y*r/2;
            vertex.position.z = vertex.normal.z*r/2;
            data.push_back(vertex);
//19        
            vertex.texCoords.x = 1;
            vertex.texCoords.y = 1.;
            //--1

            vertex.normal.x = 1;
            vertex.normal.y = -1;
            vertex.normal.z = -1;

            vertex.position.x = vertex.normal.x*r/2;
            vertex.position.y = vertex.normal.y*r/2;
            vertex.position.z = vertex.normal.z*r/2;
            data.push_back(vertex);
//20        
            vertex.texCoords.x = 0;
            vertex.texCoords.y = 1.;

            vertex.normal.x = 1;
            vertex.normal.y = -1;
            vertex.normal.z = 1;

            vertex.position.x = vertex.normal.x*r/2;
            vertex.position.y = vertex.normal.y*r/2;
            vertex.position.z = vertex.normal.z*r/2;
            data.push_back(vertex);
//21        
            vertex.texCoords.x = 1;
            vertex.texCoords.y = 0.;

            vertex.normal.x = 1;
            vertex.normal.y = 1;
            vertex.normal.z = -1;

            vertex.position.x = vertex.normal.x*r/2;
            vertex.position.y = vertex.normal.y*r/2;
            vertex.position.z = vertex.normal.z*r/2;
            data.push_back(vertex);
//22        
            vertex.texCoords.x = 0;
            vertex.texCoords.y = 1.;
            //1--

            vertex.normal.x = 1;
            vertex.normal.y = -1;
            vertex.normal.z = -1;

            vertex.position.x = vertex.normal.x*r/2;
            vertex.position.y = vertex.normal.y*r/2;
            vertex.position.z = vertex.normal.z*r/2;
            data.push_back(vertex);
//23        
            vertex.texCoords.x = 0;
            vertex.texCoords.y = 1.;

            vertex.normal.x = 1;
            vertex.normal.y = 1;
            vertex.normal.z = -1;

            vertex.position.x = vertex.normal.x*r/2;
            vertex.position.y = vertex.normal.y*r/2;
            vertex.position.z = vertex.normal.z*r/2;
            data.push_back(vertex);
//24        
            vertex.texCoords.x = 1;
            vertex.texCoords.y = 1.;

            vertex.normal.x = -1;
            vertex.normal.y = -1;
            vertex.normal.z = -1;

            vertex.position.x = vertex.normal.x*r/2;
            vertex.position.y = vertex.normal.y*r/2;
            vertex.position.z = vertex.normal.z*r/2;
            data.push_back(vertex);
//25        
            vertex.texCoords.x = 1;
            vertex.texCoords.y = 1.;
            //1--

            vertex.normal.x = 1;
            vertex.normal.y = -1;
            vertex.normal.z = -1;

            vertex.position.x = vertex.normal.x*r/2;
            vertex.position.y = vertex.normal.y*r/2;
            vertex.position.z = vertex.normal.z*r/2;
            data.push_back(vertex);
//26        
            vertex.texCoords.x = 1;
            vertex.texCoords.y = 0.;

            vertex.normal.x = 1;
            vertex.normal.y = -1;
            vertex.normal.z = 1;

            vertex.position.x = vertex.normal.x*r/2;
            vertex.position.y = vertex.normal.y*r/2;
            vertex.position.z = vertex.normal.z*r/2;
            data.push_back(vertex);
//27        
            vertex.texCoords.x = 0;
            vertex.texCoords.y = 1.;

            vertex.normal.x = -1;
            vertex.normal.y = -1;
            vertex.normal.z = -1;

            vertex.position.x = vertex.normal.x*r/2;
            vertex.position.y = vertex.normal.y*r/2;
            vertex.position.z = vertex.normal.z*r/2;
            data.push_back(vertex);
//28        
            vertex.texCoords.x = 0;
            vertex.texCoords.y = 0.;
            //1--

            vertex.normal.x = -1;
            vertex.normal.y = -1;
            vertex.normal.z = 1;

            vertex.position.x = vertex.normal.x*r/2;
            vertex.position.y = vertex.normal.y*r/2;
            vertex.position.z = vertex.normal.z*r/2;
            data.push_back(vertex);
//29        
            vertex.texCoords.x = 0;
            vertex.texCoords.y = 1.;

            vertex.normal.x = -1;
            vertex.normal.y = -1;
            vertex.normal.z = -1;

            vertex.position.x = vertex.normal.x*r/2;
            vertex.position.y = vertex.normal.y*r/2;
            vertex.position.z = vertex.normal.z*r/2;
            data.push_back(vertex);
//30        
            vertex.texCoords.x = 1;
            vertex.texCoords.y = 0.;

            vertex.normal.x = 1;
            vertex.normal.y = -1;
            vertex.normal.z = 1;

            vertex.position.x = vertex.normal.x*r/2;
            vertex.position.y = vertex.normal.y*r/2;
            vertex.position.z = vertex.normal.z*r/2;
            data.push_back(vertex);
//31        
            vertex.texCoords.x = 1;
            vertex.texCoords.y = 1.;
            //-1-

            vertex.normal.x = -1;
            vertex.normal.y = -1;
            vertex.normal.z = 1;

            vertex.position.x = vertex.normal.x*r/2;
            vertex.position.y = vertex.normal.y*r/2;
            vertex.position.z = vertex.normal.z*r/2;
            data.push_back(vertex);
//32        
            vertex.texCoords.x = 0;
            vertex.texCoords.y = 1.;

            vertex.normal.x = -1;
            vertex.normal.y = -1;
            vertex.normal.z = -1;

            vertex.position.x = vertex.normal.x*r/2;
            vertex.position.y = vertex.normal.y*r/2;
            vertex.position.z = vertex.normal.z*r/2;
            data.push_back(vertex);
//33        
            vertex.texCoords.x = 1;
            vertex.texCoords.y = 0.;

            vertex.normal.x = -1;
            vertex.normal.y = 1;
            vertex.normal.z = 1;

            vertex.position.x = vertex.normal.x*r/2;
            vertex.position.y = vertex.normal.y*r/2;
            vertex.position.z = vertex.normal.z*r/2;
            data.push_back(vertex);
//34        
            vertex.texCoords.x = 0;
            vertex.texCoords.y = 1.;
            //-1-

            vertex.normal.x = -1;
            vertex.normal.y = -1;
            vertex.normal.z = 1;

            vertex.position.x = vertex.normal.x*r/2;
            vertex.position.y = vertex.normal.y*r/2;
            vertex.position.z = vertex.normal.z*r/2;
            data.push_back(vertex);
//35        
            vertex.texCoords.x = 1;
            vertex.texCoords.y = 1.;

            vertex.normal.x = 1;
            vertex.normal.y = -1;
            vertex.normal.z = 1;

            vertex.position.x = vertex.normal.x*r/2;
            vertex.position.y = vertex.normal.y*r/2;
            vertex.position.z = vertex.normal.z*r/2;
            data.push_back(vertex);
//36        
            vertex.texCoords.x = 0;
            vertex.texCoords.y = 0.;

            vertex.normal.x = -1;
            vertex.normal.y = 1;
            vertex.normal.z = 1;

            vertex.position.x = vertex.normal.x*r/2;
            vertex.position.y = vertex.normal.y*r/2;
            vertex.position.z = vertex.normal.z*r/2;
            data.push_back(vertex);
            //-1-

            
    m_nVertexCount = 36;
    
    GLuint idx = 0;
    // Construit les vertex finaux en regroupant les données en triangles:
    // 
    for(GLsizei j = 0; j < 12; ++j) {
        GLsizei offset = j*3;
        //std::cout << data[offset].position << std::endl;
        m_Vertices.push_back(data[offset]);
        m_Vertices.push_back(data[offset + 1]);
        m_Vertices.push_back(data[offset + 2]);
    }
    
    // Attention ! dans cette implantation on duplique beaucoup de sommets. Une meilleur stratégie est de passer
    // par un Index Buffer Object, que nous verrons dans les prochains TDs
}

}
