#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <iostream>


struct TrackBallCamera{
    public:
    TrackBallCamera():
        m_fDistance {-5.}, m_fAngleX {0.}, m_fAngleY {0.}
    {}
    void moveFront(float delta){
        m_fDistance += delta;
    }

    void rotateLeft(float degrees){
        m_fAngleX += degrees;
    }

    void rotateUp(float degrees){
        m_fAngleY += degrees;
    }

    glm::mat4 getViewMatrix() const{
        return glm::rotate(glm::rotate(glm::translate(glm::mat4(1.0),glm::vec3(0., 0., m_fDistance)), glm::radians(m_fAngleX),glm::vec3(1., 0., 0.)), glm::radians(m_fAngleY),glm::vec3(0., 1., 0.));
    }

    private:
        float m_fDistance;
        float m_fAngleX;
        float m_fAngleY;
};