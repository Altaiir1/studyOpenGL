//
// Created by Lemi Yürekli on 19.12.25.
//

#pragma once

#include "VBO.h"

class VAO
{
public:
    GLuint ID;

    VAO();

    void LinkVBO(VBO VBO, GLuint layout);

    void Bind();
    void Unbind();
    void Delete();
};