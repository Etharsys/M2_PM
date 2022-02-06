#include "glm.hpp"
#include "Camera.hpp"

void Camera::computeDirectionVectors()
{
    m_FrontVector = glm::vec3(cos(m_fTheta) * sin(m_fPhi), sin(m_fTheta), cos(m_fTheta) * cos(m_fPhi));
    m_LeftVector  = glm::vec3(sin(m_fPhi+M_PI/2), 0, cos(m_fPhi+M_PI/2));
    m_UpVector    = glm::cross(m_FrontVector, m_LeftVector);
}

void Camera::zoom(float t)
{
    m_fDistance += t;
}

void Camera::moveLeft(float t)
{
    m_Position += t * m_LeftVector;
}

void Camera::moveFront(float t)
{
    m_Position += t * glm::normalize(glm::vec3(m_FrontVector.x, 0, m_FrontVector.z));
}

void Camera::moveUp(float t)
{
    m_Position += t * glm::normalize(glm::vec3(0, m_UpVector.y, 0));
}

void Camera::rotateLeft(float degrees)
{
    float rad = glm::radians(degrees);
    m_fPhi += rad;
    computeDirectionVectors();
}

void Camera::rotateUp(float degrees)
{
    float rad = glm::radians(degrees);
    m_fTheta += rad;
    computeDirectionVectors();
}

glm::mat4 Camera::getViewMatrix() const
{
    auto eye = m_Position - m_FrontVector * m_fDistance;
    auto v = m_Position + m_FrontVector;

    glm::mat4 mat = glm::lookAt(eye, v, m_UpVector);

    return mat;
}