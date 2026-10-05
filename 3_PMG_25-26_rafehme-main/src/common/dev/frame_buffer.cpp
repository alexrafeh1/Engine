#include "Motor/common/dev/frame_buffer.h"

#include <iostream>

Framebuffer::~Framebuffer()
{
    Destroy();
}

Framebuffer::Framebuffer(Framebuffer&& other) noexcept
    : framebuffer_(other.framebuffer_),
    color_texture_(other.color_texture_),
    depth_texture_(other.depth_texture_),
    width_(other.width_),
    height_(other.height_)
{
    other.framebuffer_ = 0;
    other.color_texture_ = 0;
    other.depth_texture_ = 0;
    other.width_ = 0;
    other.height_ = 0;
}

Framebuffer& Framebuffer::operator=(Framebuffer&& other) noexcept
{
    if (this != &other)
    {
        Destroy();

        framebuffer_ = other.framebuffer_;
        color_texture_ = other.color_texture_;
        depth_texture_ = other.depth_texture_;
        width_ = other.width_;
        height_ = other.height_;

        other.framebuffer_ = 0;
        other.color_texture_ = 0;
        other.depth_texture_ = 0;
        other.width_ = 0;
        other.height_ = 0;
    }

    return *this;
}

bool Framebuffer::Create(int width, int height, FramebufferTextureType type)
{
    if (width <= 0 || height <= 0)
        return false;

    Destroy();

    width_ = width;
    height_ = height;
    type_ = type;

    glCreateFramebuffers(1, &framebuffer_ );

    return CreateTextures() && IsComplete();
}
bool Framebuffer::CreateTextures()
{
    if (type_ == FramebufferTextureType::Color)
    {
        glCreateTextures(
            GL_TEXTURE_2D,
            1,
            &color_texture_
        );

        glTextureStorage2D(
            color_texture_,
            1,
            GL_RGBA8,
            width_,
            height_
        );

        glTextureParameteri(
            color_texture_,
            GL_TEXTURE_MIN_FILTER,
            GL_LINEAR
        );

        glTextureParameteri(
            color_texture_,
            GL_TEXTURE_MAG_FILTER,
            GL_LINEAR
        );

        glTextureParameteri(
            color_texture_,
            GL_TEXTURE_WRAP_S,
            GL_CLAMP_TO_EDGE
        );

        glTextureParameteri(
            color_texture_,
            GL_TEXTURE_WRAP_T,
            GL_CLAMP_TO_EDGE
        );

        glNamedFramebufferTexture(
            framebuffer_,
            GL_COLOR_ATTACHMENT0,
            color_texture_,
            0
        );

        constexpr GLenum drawBuffers[] =
        {
            GL_COLOR_ATTACHMENT0
        };

        glNamedFramebufferDrawBuffers(
            framebuffer_,
            1,
            drawBuffers
        );
    }

    if (type_ == FramebufferTextureType::Depth)
    {
        glCreateTextures(
            GL_TEXTURE_2D,
            1,
            &depth_texture_
        );

        glTextureStorage2D(
            depth_texture_,
            1,
            GL_DEPTH_COMPONENT32F,
            width_,
            height_
        );

        glTextureParameteri(
            depth_texture_,
            GL_TEXTURE_MIN_FILTER,
            GL_NEAREST
        );

        glTextureParameteri(
            depth_texture_,
            GL_TEXTURE_MAG_FILTER,
            GL_NEAREST
        );

        glTextureParameteri(
            depth_texture_,
            GL_TEXTURE_WRAP_S,
            GL_CLAMP_TO_BORDER
        );

        glTextureParameteri(
            depth_texture_,
            GL_TEXTURE_WRAP_T,
            GL_CLAMP_TO_BORDER
        );

        constexpr float borderColor[] =
        {
            1.0f,
            1.0f,
            1.0f,
            1.0f
        };

        glTextureParameterfv(
            depth_texture_,
            GL_TEXTURE_BORDER_COLOR,
            borderColor
        );

        glNamedFramebufferTexture(
            framebuffer_,
            GL_DEPTH_ATTACHMENT,
            depth_texture_,
            0
        );

        glNamedFramebufferDrawBuffer(
            framebuffer_,
            GL_NONE
        );

        glNamedFramebufferReadBuffer(
            framebuffer_,
            GL_NONE
        );
    }

    return true;
}


void Framebuffer::Resize(int width, int height)
{
    if (width <= 0 || height <= 0)
        return;

    if (width == width_ && height == height_)
        return;

    Create(width, height,type_);
}

void Framebuffer::Bind()
{
    if (bound_)
        return;

    // Guardamos el framebuffer que estaba activo
    glGetIntegerv(
        GL_DRAW_FRAMEBUFFER_BINDING,
        &previous_framebuffer_
    );

    // Guardamos el viewport que estaba activo
    glGetIntegerv(
        GL_VIEWPORT,
        previous_viewport_
    );

    // Activamos nuestro framebuffer
    glBindFramebuffer(
        GL_FRAMEBUFFER,
        framebuffer_
    );

    // Usamos su tamaño
    glViewport(
        0,
        0,
        width_,
        height_
    );

    bound_ = true;
}

void Framebuffer::Unbind()
{
    if (!bound_)
        return;

    // Restauramos el framebuffer anterior
    glBindFramebuffer(
        GL_FRAMEBUFFER,
        previous_framebuffer_
    );

    // Restauramos el viewport anterior
    glViewport(
        previous_viewport_[0],
        previous_viewport_[1],
        previous_viewport_[2],
        previous_viewport_[3]
    );

    bound_ = false;
}

bool Framebuffer::IsComplete() const
{
    return glCheckNamedFramebufferStatus(framebuffer_, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

void Framebuffer::Destroy()
{
    if (depth_texture_) {glDeleteTextures(  1,  &depth_texture_);
        depth_texture_ = 0;
    }

    if (color_texture_)
    {
        glDeleteTextures(   1,&color_texture_ );
        color_texture_ = 0;
    }

    if (framebuffer_)
    {
        glDeleteFramebuffers(  1,&framebuffer_);
        framebuffer_ = 0;
    }

    width_ = 0;
    height_ = 0;
}