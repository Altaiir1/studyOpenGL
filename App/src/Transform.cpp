#include "Transform.h"

Transform::Transform()
{
    m_Position = glm::vec3(0.0f);
    m_Rotation = glm::vec3(0.0f);
    m_Scale = glm::vec3(1.0f);
}

Transform::~Transform()
{
}

void Transform::setPosition(glm::vec3 position)
{
    m_Position = position;
}

glm::vec3 Transform::getPosition() const
{
    return m_Position;
}

void Transform::setRotation(glm::vec3 rotation)
{
    m_Rotation = rotation;
}

glm::vec3 Transform::getRotation() const
{
    return m_Rotation;
}

void Transform::setScale(glm::vec3 scale)
{
    m_Scale = scale;
}

glm::vec3 Transform::getScale() const
{
    return m_Scale;
}

glm::mat4 Transform::getMatrix() const
{
    // Identity Matrix
    glm::mat4 matrix = glm::mat4(1.0f);

    // Translate
    matrix = glm::translate(matrix, m_Position);

    // Rotation
    matrix = glm::rotate(matrix, m_Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    matrix = glm::rotate(matrix, m_Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    matrix = glm::rotate(matrix, m_Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

    // Scale
    matrix = glm::scale(matrix, m_Scale);

    return matrix;
}
