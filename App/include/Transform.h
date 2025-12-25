#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Transform
{
private:
    glm::vec3 m_Position;
    glm::vec3 m_Rotation;
    glm::vec3 m_Scale;

    glm::vec3 m_Speed;
    bool m_AutoRotate;

public:
    Transform();
    ~Transform();

    void setPosition(glm::vec3 position);
    glm::vec3 getPosition() const;

    void setRotation(glm::vec3 rotation);
    glm::vec3 getRotation() const;

    void setScale(glm::vec3 scale);
    glm::vec3 getScale() const;

    glm::mat4 getMatrix() const;

    void update(float deltaTime);

    void setAutoRotate(bool autoRotate);
    bool getAutoRotate() const;

    void setSpeed(glm::vec3 speed);
    glm::vec3 getSpeed() const;
};