#include "Motor/common/dev/texture.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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

Texture::Texture(Texture&& other) noexcept
    : id_(other.id_),
    name_(std::move(other.name_))
{
    other.id_ = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept
{
    if (this == &other)
        return *this;

    if (glIsTexture(id_))
        glDeleteTextures(1, &id_);

    id_ = other.id_;
    name_ = std::move(other.name_);

    other.id_ = 0;

    return *this;
}

std::optional<Texture> Texture::LoadTexture(std::string_view path)
{
    Texture texture;
    std::string string_path(path);
    texture.name_ = string_path;

    int x, y, channels;

    unsigned char* data = stbi_load(string_path.c_str(), &x, &y, &channels, 0);

    if (!data)
    {
        return std::nullopt;
    }

    GLenum format;

    switch (channels)
    {
    case 1: format = GL_RED; break;
    case 2: format = GL_RG; break;
    case 3: format = GL_RGB; break;
    case 4: format = GL_RGBA; break;
    default:
        stbi_image_free(data);
        return std::nullopt;
    }

    glBindTexture(GL_TEXTURE_2D, texture.id_);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        format,
        x,
        y,
        0,
        format,
        GL_UNSIGNED_BYTE,
        data
    );

    glGenerateMipmap(GL_TEXTURE_2D);

    glFlush();

    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

std::optional<Texture> Texture::LoadEmbeddedTexture(const aiTexture* embedded)
{

    if (!embedded)
        return std::nullopt;

    int width = 0;
    int height = 0;
    int channels = 0;

    unsigned char* data = nullptr;

    if (embedded->mHeight == 0)
    {
        // PNG, JPEG, etc.
        data = stbi_load_from_memory(
            reinterpret_cast<const stbi_uc*>(embedded->pcData),
            static_cast<int>(embedded->mWidth),
            &width,
            &height,
            &channels,
            4
        );
    }
    else
    {
        // Textura sin comprimir: aiTexel (BGRA)
        width = static_cast<int>(embedded->mWidth);
        height = static_cast<int>(embedded->mHeight);
        channels = 4;

        data = new unsigned char[width * height * 4];

        for (int i = 0; i < width * height; ++i)
        {
            data[i * 4 + 0] = embedded->pcData[i].r;
            data[i * 4 + 1] = embedded->pcData[i].g;
            data[i * 4 + 2] = embedded->pcData[i].b;
            data[i * 4 + 3] = embedded->pcData[i].a;
        }
    }

    if (!data)
        return std::nullopt;

    Texture texture;

    glBindTexture(GL_TEXTURE_2D, texture.id_);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA8,
        width,
        height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        data
    );

    glGenerateMipmap(GL_TEXTURE_2D);

    if (embedded->mHeight == 0)
        stbi_image_free(data);
    else
        delete[] data;

    return texture;
}