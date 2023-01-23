#include "img/src/SizeU.h"
#if defined(COOL_OPENGL)

#include "../Texture.h"
#include <img/img.hpp>

namespace Cool::OpenGL {

Texture::~Texture()
{
    DestroyTexture(m_textureID);
}

Texture::Texture(Texture&& rhs) noexcept
    : m_textureID{rhs.m_textureID}
    , _aspect_ratio{rhs._aspect_ratio}
{
    rhs.m_textureID = static_cast<GLuint>(-1);
}

Texture& Texture::operator=(Texture&& rhs) noexcept
{
    if (this != &rhs)
    {
        m_textureID     = rhs.m_textureID;
        _aspect_ratio   = rhs._aspect_ratio;
        rhs.m_textureID = static_cast<GLuint>(-1);
    }
    return *this;
}

GLuint Texture::CreateTextureID(GLint interpolationMode, GLint wrapMode)
{
    GLuint textureID; // NOLINT
    GLDebug(glGenTextures(1, &textureID));
    GLDebug(glBindTexture(GL_TEXTURE_2D, textureID));
    GLDebug(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interpolationMode));
    GLDebug(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interpolationMode));
    GLDebug(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode));
    GLDebug(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode));
    GLDebug(glBindTexture(GL_TEXTURE_2D, 0));
    return textureID;
}

void Texture::DestroyTexture(GLuint texID)
{
    GLDebug(glDeleteTextures(1, &texID));
}

Texture::Texture(std::filesystem::path filepath, GLint interpolationMode, GLint wrapMode)
{
    m_textureID = LoadTexture(filepath, interpolationMode, wrapMode, &_aspect_ratio);
#if DEBUG
    m_bDataUploaded = true;
#endif
}

GLuint Texture::LoadTexture(std::filesystem::path filepath, GLint interpolationMode, GLint wrapMode, float* out_aspect_ratio)
{
    // Load image
    const auto image = img::load(filepath, 4);
    // Create texture
    GLuint texID = Texture::CreateTextureID(interpolationMode, wrapMode);
    // Upload data
    GLDebug(glBindTexture(GL_TEXTURE_2D, texID));
    GLDebug(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, static_cast<GLsizei>(image.size().width()), static_cast<GLsizei>(image.size().height()), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data()));
    //
    if (out_aspect_ratio)
    {
        *out_aspect_ratio = img::SizeU::aspect_ratio(image.size());
    }

    return texID;
}

void Texture::genTexture(GLint interpolationMode, GLint wrapMode)
{
#if DEBUG
    if (m_textureID != static_cast<GLuint>(-1))
    {
        Log::Debug::error("Texture::genTexture", "You have already generated that texture!");
    }
#endif
    m_textureID = CreateTextureID(interpolationMode, wrapMode);
}

void Texture::uploadRGB(int width, int height, unsigned char* data)
{
#if DEBUG
    m_bDataUploaded = true;
    if (m_textureID == static_cast<GLuint>(-1))
    {
        Log::Debug::error("Texture::uploadRGB", "You haven't generated that texture yet!");
    }
#endif
    GLDebug(glBindTexture(GL_TEXTURE_2D, m_textureID));
    GLDebug(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
    GLDebug(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::uploadRGBA(int width, int height, unsigned char* data)
{
#if DEBUG
    m_bDataUploaded = true;
    if (m_textureID == static_cast<GLuint>(-1))
    {
        Log::Debug::error("Texture::uploadRGBA", "You haven't generated that texture yet!");
    }
#endif
    GLDebug(glBindTexture(GL_TEXTURE_2D, m_textureID));
    GLDebug(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
    GLDebug(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::attachToSlot(int slot) const
{
#if DEBUG
    if (m_textureID == static_cast<GLuint>(-1))
    {
        Log::Debug::error("Texture::attachToSlot", "You haven't generated that texture yet!");
    }
    if (!m_bDataUploaded)
    {
        Log::Debug::error("Texture::attachToSlot", "You must upload some data (at least a width and height) before using the texture.");
    }
#endif
    GLDebug(glActiveTexture(GL_TEXTURE0 + static_cast<GLenum>(slot)));
    GLDebug(glBindTexture(GL_TEXTURE_2D, m_textureID));
}

} // namespace Cool::OpenGL

#endif