//
// Created by Lemi Yürekli on 19.12.25.
//

#pragma once

#include <glad/glad.h>

class EBO
{
public:
    GLuint ID;

    EBO(GLuint* indices, GLsizeiptr size);

    void Bind();
    void Unbind();
    void Delete();
};