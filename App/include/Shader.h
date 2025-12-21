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

    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;

};