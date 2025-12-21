#include "Textures/Texture.h"
#include "utils/Debug.h"
#include "stb_image/stb_image.h"

#include <iostream>

Texture::Texture(const char *imagePath, GLenum textureType, GLenum wrapS,
                 GLenum wrapT, GLenum minFilter, GLenum magFilter)
    : type(textureType), width(0), height(0), nrChannels(0)
{
    glGenTextures(1, &ID);
    glBindTexture(type, ID);

    loadFromFile(imagePath, wrapS, wrapT, minFilter, magFilter);
}

Texture::~Texture()
{
    Delete();
}

void Texture::Bind() const
{
    GLCall(glBindTexture(type, ID));
}

void Texture::Unbind() const
{
    GLCall(glBindTexture(type, 0));
}

void Texture::Activate(GLenum slot) const
{
    GLCall(glActiveTexture(slot));
    Bind();
}

void Texture::Delete()
{
    if (ID != 0)
    {
        GLCall(glDeleteTextures(1, &ID));
        ID = 0;
    }
}

void Texture::loadFromFile(const char *imagePath, GLenum wrapS, GLenum wrapT,
                           GLenum minFIlter, GLenum magFilter)
{
    // Set texture parameters
    GLCall(glTexParameteri(type, GL_TEXTURE_WRAP_S, wrapS));
    GLCall(glTexParameteri(type, GL_TEXTURE_WRAP_T, wrapT));
    GLCall(glTexParameteri(type, GL_TEXTURE_MIN_FILTER, minFIlter));
    GLCall(glTexParameteri(type, GL_TEXTURE_MAG_FILTER, magFilter));

    // Load image
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(imagePath, &width, &height, &nrChannels, 0);

    if (data)
    {
        // Determine format based on number of channels
        GLenum format = GL_RGB;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        GLCall(
            glTexImage2D(type, 0, format, width, height, 0, format,
                GL_UNSIGNED_BYTE, data));
        GLCall(glGenerateMipmap(type));

        std::cout << "Texture loaded: " << imagePath << " (" << width << "x" <<
                height << ", " << nrChannels << " channels)\n";
    } else
    {
        std::cerr << "Failed to load texture: " << imagePath << " - " <<
                stbi_failure_reason() << '\n';
    }

    stbi_image_free(data);
}
