#pragma once

#include <math.h>

#include <glm/glm.hpp>

class Camera {

    public:

        Camera(glm::vec3 position0)
            : m_Position { position0 }, m_fPhi { M_PI }, m_fTheta { 0 }, m_fDistance { 5 }
        {
            computeDirectionVectors();
        }

        void computeDirectionVectors();

        void zoom(float t);

        void moveLeft(float t);
        
        void moveFront(float t);

        void moveUp(float t);

        void rotateLeft(float degrees);
        
        void rotateUp(float degrees);

        glm::mat4 getViewMatrix() const;


        glm::vec3 getPosition() { return m_Position; };
        

    private:

        glm::vec3 m_Position;       // camera position
        float m_fDistance;          // distance between target point and view point
        float m_fPhi;               // rotation on X axis
        float m_fTheta;             // rotation on Y axis

        glm::vec3 m_FrontVector;
        glm::vec3 m_LeftVector;
        glm::vec3 m_UpVector;

};