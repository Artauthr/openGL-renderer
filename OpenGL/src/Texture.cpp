#include "Texture.h"

Texture::Texture(std::string path)
	: m_Path(path), ID(0)
{
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


    stbi_set_flip_vertically_on_load(1);
    int width, height, nrChannels;
    unsigned char* loadBuffer = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

    if (loadBuffer)
    {
        GLenum format = GL_RGB;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

     
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, loadBuffer);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(loadBuffer);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, 0); 
        throw std::invalid_argument("Failed loading texture with path: " + path);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
    glDeleteTextures(1, &ID);
}

void Texture::Bind(unsigned int unit) const
{
    if (unit >= GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS)
    {
        throw std::invalid_argument("Texture unit out of range");
    }

    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
