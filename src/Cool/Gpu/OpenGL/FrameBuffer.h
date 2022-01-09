#pragma once
#if defined(COOL_OPENGL)

#include <img/img.hpp>

namespace Cool {

static constexpr GLuint SCREEN_FRAMEBUFFER_ID = 0;

class FrameBuffer {
public:
    FrameBuffer();
    virtual ~FrameBuffer();

    void setSize(img::Size size);

    void bind() const;
    void unbind() const;
    /// <summary>
    /// Copies the content of this framebuffer to another framebuffer
    /// </summary>
    /// <param name="botLeft">Coordinates of the bottom left corner of the area to copy (in pixels)</param>
    /// <param name="topRight">Coordinates of the top right corner of the area to copy (in pixels)</param>
    /// <param name="dstFrameBufferID">OpenGL ID of the target framebuffer</param>
    /// <param name="interpolationMode">The OpenGL interpolation mode (how the image will be resized if needed).
    /// The default value is GL_LINEAR.
    /// Can be either GL_LINEAR or GL_NEAREST (nearest neighbour, a.k.a. no real interpolation)
    /// </param>
    void blitTo(const glm::ivec2& botLeft, const glm::ivec2& topRight, GLuint dstFrameBufferID = SCREEN_FRAMEBUFFER_ID, GLenum interpolationMode = GL_LINEAR) const;
    /// <summary>
    /// Copies the content of this framebuffer to another framebuffer
    /// </summary>
    /// <param name="interpolationMode">The OpenGL interpolation mode (how the image will be resized if needed).
    /// The default value is GL_LINEAR.
    /// Can be either GL_LINEAR or GL_NEAREST (nearest neighbour, a.k.a. no real interpolation)
    /// </param>
    void blitTo(FrameBuffer& frameBuffer, GLenum interpolationMode = GL_LINEAR) const;

    inline img::Size::DataType width() const { return m_size.width(); }
    inline img::Size::DataType height() const { return m_size.height(); }
    inline img::Size           size() const { return m_size; }
    inline float               aspectRatio() const { return img::SizeU::aspect_ratio(m_size); }

protected:
    virtual void destroyAttachments();
    virtual void createAttachments(img::Size size);
    virtual void attachAttachments();

    inline GLuint frameBufferId() { return m_frameBufferId; }

private:
    GLuint      m_frameBufferId       = static_cast<GLuint>(-1);
    GLuint      m_depthRenderBufferId = static_cast<GLuint>(-1);
    img::Size   m_size;
    mutable int m_prevViewport[4] = {};
};

} // namespace Cool

#endif