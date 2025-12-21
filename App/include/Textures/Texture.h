#pragma once

#include <glad/glad.h>
#include <string>

class Texture
{
public:
    GLuint ID;

    explicit Texture(const char *imagePath, GLenum textureType = GL_TEXTURE_2D,
                     GLenum wrapS = GL_REPEAT, GLenum wrapT = GL_REPEAT,
                     GLenum minFilter = GL_LINEAR_MIPMAP_LINEAR,
                     GLenum magFilter = GL_LINEAR);

    ~Texture();

    Texture(const Texture &) = delete;

    Texture &operator=(const Texture &) = delete;

    void Bind() const;

    void Unbind() const;

    void Activate(GLenum slot) const;

    void Delete();

private:
    GLenum type;
    int width, height, nrChannels;

    void loadFromFile(const char *imagePath, GLenum wrapS, GLenum wrapT,
                      GLenum minFIlter, GLenum magFilter);
};
