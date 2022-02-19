#include <cmath>
#include <vector>
#include <iostream>
#include "glimac/common.hpp"
#include "glimac/Cylinder.hpp"

namespace glimac {

void Cylinder::build(GLsizei s) {
    GLfloat sizefragment = 1.f /s;
    GLfloat dTheta = 2* glm::pi<float>() * sizefragment;

    std::vector<ShapeVertex> data;
    
    ShapeVertex vertex;
    vertex.texCoords.x = 0;
    vertex.texCoords.y = 1.;
    

    
    for (int i = 0; i < s; i++){
        // 1er cercle
        //sommet centrale
        vertex.normal.x = 0;
        vertex.normal.y = 1;
        vertex.normal.z = 0;


        vertex.position.x = 0;
        vertex.position.y = 1./2;
        vertex.position.z = 0;
        data.push_back(vertex);

        //sommets du cercle


        vertex.position.x = sin(i * dTheta);
        vertex.position.y = 1./2;
        vertex.position.z = cos(i * dTheta);
        data.push_back(vertex);

        vertex.position.x = sin((i+1) * dTheta);
        vertex.position.y = 1./2;
        vertex.position.z = cos((i+1) * dTheta);
        data.push_back(vertex);



        vertex.normal.y = -1;

        vertex.position.x = 0;
        vertex.position.y = -1./2;
        vertex.position.z = 0;
        data.push_back(vertex);

        vertex.position.x = sin(i * dTheta);
        vertex.position.y = -1./2;
        vertex.position.z = cos(i * dTheta);
        data.push_back(vertex);

        vertex.position.x = sin((i+1) * dTheta);
        vertex.position.y = -1./2;
        vertex.position.z = cos((i+1) * dTheta);
        data.push_back(vertex);


        //triangle a l'endroit

        vertex.normal.y = -1;

        vertex.normal.x = sin(i * dTheta);
        vertex.normal.z = cos(i * dTheta);


        vertex.position.x = vertex.normal.x;;
        vertex.position.y = -1./2;// 
        vertex.position.z = vertex.normal.z;
        data.push_back(vertex);

        vertex.normal.x = sin((i +1)* dTheta);
        vertex.normal.z = cos((i+1) * dTheta);

        vertex.position.x = vertex.normal.x;;
        vertex.position.y = -1./2;// 
        vertex.position.z = vertex.normal.z;
        data.push_back(vertex);

        vertex.normal.y = 1;
        vertex.normal.x = sin(i * dTheta);
        vertex.normal.z = cos(i * dTheta);

        vertex.position.x = vertex.normal.x;;
        vertex.position.y = 1./2;;// 
        vertex.position.z = vertex.normal.z;
        data.push_back(vertex);

        //triangle a l'envers
        vertex.normal.x = sin(i * dTheta);
        vertex.normal.z = cos(i * dTheta);


        vertex.position.x = vertex.normal.x;;
        vertex.position.y = 1./2;// 
        vertex.position.z = vertex.normal.z;
        data.push_back(vertex);

        vertex.normal.x = sin((i+1)* dTheta);
        vertex.normal.z = cos((i+1) * dTheta);

        vertex.position.x = vertex.normal.x;;
        vertex.position.y = 1./2;// 
        vertex.position.z = vertex.normal.z;
        data.push_back(vertex);

        vertex.normal.y = -1;

        vertex.position.x = vertex.normal.x;;
        vertex.position.y = -1./2;;// 
        vertex.position.z = vertex.normal.z;
        data.push_back(vertex);

    }
    /*
   for (int i = 0; i < s; i++){
        GLfloat cosTheta = cos(-glm::pi<float>() / 2 + i * dTheta);
        vertex.normal.y = -1;

        vertex.normal.x = sin(i * dTheta)*cosTheta;
        vertex.normal.z = cos(i * dTheta)*cosTheta;


        vertex.position.x = r*vertex.normal.x;;
        vertex.position.y = vertex.normal.y*h/2;// 
        vertex.position.z = r*vertex.normal.z;
        data.push_back(vertex);

        vertex.normal.x = sin((i +1)* dTheta)*cosTheta;
        vertex.normal.z = cos((i+1) * dTheta)*cosTheta;

        vertex.position.x = r*vertex.normal.x;;
        vertex.position.y = vertex.normal.y*h/2;// 
        vertex.position.z = r*vertex.normal.z;
        data.push_back(vertex);

        vertex.normal.y = 1;
        vertex.normal.x = sin(i * dTheta)*cosTheta;
        vertex.normal.z = cos(i * dTheta)*cosTheta;

        vertex.position.x = r*vertex.normal.x;;
        vertex.position.y = vertex.normal.y*h/2;;// 
        vertex.position.z = r*vertex.normal.z;
        data.push_back(vertex);
////////////////////////////////////////////////////////////////////////////

        vertex.normal.x = sin(i * dTheta)*cosTheta;
        vertex.normal.z = cos(i * dTheta)*cosTheta;

        vertex.position.x = r*vertex.normal.x;;
        vertex.position.y = vertex.normal.y*h/2;;// +h/2
        vertex.position.z = r*vertex.normal.z;
        data.push_back(vertex);

        vertex.normal.x = sin((i+1) * dTheta)*cosTheta;
        vertex.normal.z = cos((i+1) * dTheta)*cosTheta;

        vertex.position.x = r*vertex.normal.x;;
        vertex.position.y = vertex.normal.y*h/2;;// +h/2
        vertex.position.z = r*vertex.normal.z;
        data.push_back(vertex);

        vertex.normal.y = -1;
        
        vertex.position.x = r*vertex.normal.x;;
        vertex.position.y = vertex.normal.y*h/2;;// -h/2
        vertex.position.z = r*vertex.normal.z;
        data.push_back(vertex);
   }  
    }*/
    

    


    m_nVertexCount = s*12;
    
    GLuint idx = 0;
    // Construit les vertex finaux en regroupant les données en triangles:
    // 
    for(GLsizei j = 0; j < s*12; ++j) {
    GLsizei offset = j*3;
    //std::cout << data[offset].position << std::endl;
    m_Vertices.push_back(data[offset]);
    m_Vertices.push_back(data[offset + 1]);
    m_Vertices.push_back(data[offset +2]);
    }
    
    // Attention ! dans cette implantation on duplique beaucoup de sommets. Une meilleur stratégie est de passer
    // par un Index Buffer Object, que nous verrons dans les prochains TDs
}

}
