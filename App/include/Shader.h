//
// Created by Lemi Yürekli on 19.12.25.
//

#pragma once

#include "glad/glad.h"


#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

std::string get_file_contents(const char* filename);

class Shader
{
public:
    GLuint ID;

    Shader(const char* vertexFile, const char* fragmentFile);

    void Activate() const;
    void Delete() const;
};