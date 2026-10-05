#include "Motor/common/dev/texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../deps/stb/stb_image.h"

Texture::Texture()
{
    glGenTextures(1, &id_);
}

Texture::~Texture()
{
    if (glIsTexture(id_)) {
        glDeleteTextures(1, &id_);
    }
}

Texture::Texture(Texture&& other) 
    : id_(other.id_),
      name_(other.name_)
{
    other.id_ = 0;
}

Texture& Texture::operator=(Texture&& other) 
{
    if(glIsTexture(id_)) {
       glDeleteTextures(1, &id_);
    }

    id_ = other.id_;
    name_ = other.name_;
    other.id_ = 0;

    return *this;
}



std::optional<Texture> Texture::LoadTexture(std::string_view path)
{
    Texture texture;
    std::string string_path = std::string(path);
    texture.name_ = string_path;

    int x;
    int y;
    int chanels;
    unsigned char *data_ = stbi_load(string_path.c_str(), &x, &y, &chanels, 0);
    if (!data_) {
        return std::nullopt;
    }

    glBindTexture(GL_TEXTURE_2D, texture.id_);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    GLenum format = GL_RGB;
    if (chanels == 4) format = GL_RGBA;

    glTexImage2D(GL_TEXTURE_2D,0,format,x,y,0,format,GL_UNSIGNED_BYTE,data_);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data_);
    data_ = nullptr;

    return texture;
}

