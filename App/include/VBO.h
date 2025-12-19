//
// Created by Lemi Yürekli on 19.12.25.
//

#pragma once

#include <glad/glad.h>

class VBO
{
public:
    GLuint ID;

    VBO(GLfloat* vertices, GLsizeiptr size);

    void Bind();
    void Unbind();
    void Delete();
};