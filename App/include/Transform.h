#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Transform
{
private:
    float positionX{}, positionY {};
    float rotationX{}, rotationY{};
    float scaleX{}, scaleY{};

public:
    Transform();
    ~Transform();
};