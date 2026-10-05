#ifndef __FRAME_BUFFER_H__
#define __FRAME_BUFFER_H__ 1


#include <glad/gl.h>
#include "../glm/glm.hpp"

enum class FramebufferTextureType
{
    Color,
    Depth
};

class Framebuffer
{
public:
    Framebuffer() = default;
    ~Framebuffer();

    Framebuffer(const Framebuffer&) = delete;
    Framebuffer& operator=(const Framebuffer&) = delete;

    Framebuffer(Framebuffer&& other) noexcept;
    Framebuffer& operator=(Framebuffer&& other) noexcept;

    bool Create(int width, int height,FramebufferTextureType type);
    void Resize(int width, int height);

    void Bind();
    void Unbind();

    GLuint GetColorTexture() const
    {
        return color_texture_;
    }

    GLuint GetDepthTexture() const
    {
        return depth_texture_;
    }

    GLuint GetHandle() const
    {
        return framebuffer_;
    }

    int GetWidth() const
    {
        return width_;
    }

    int GetHeight() const
    {
        return height_;
    }

    bool IsComplete() const;

private:
    void Destroy();
    bool CreateTextures();

private:
    GLuint framebuffer_ = 0;
    GLuint color_texture_ = 0;
    GLuint depth_texture_ = 0;

    FramebufferTextureType type_;
    int width_ = 0;
    int height_ = 0;

    GLint previous_framebuffer_ = 0;
    GLint previous_viewport_[4] = { 0, 0, 0, 0 };

    bool bound_ = false;
};

#endif // !__FRAME_BUFFER_H__
