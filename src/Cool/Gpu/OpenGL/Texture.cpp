#include "glpp/Functions/Texture.h"
#include "glpp/Enums/TextureKind.h"
#if defined(COOL_OPENGL)

#include <img/img.hpp>
#include "../Texture.h"
#include "img/src/SizeU.h"

namespace Cool::OpenGL {

Texture::Texture(Config config)
{
    set_wrap_mode(config.wrap_mode);
    set_interpolation_mode(config.interpolation_mode);
}

Texture::Texture(img::Size const& size, Config config)
    : Texture{config}
{
    set_size(size);
}

Texture::Texture(img::Image const& image, Config config)
    : Texture{config}
{
    set_image(image);
}

void Texture::set_size(img::Size const&)
{
    glpp::
}

void Texture::set_image(img::Image const&)
{
}

void Texture::set_interpolation_mode(glpp::Interpolation interpolation_mode)
{
}

void Texture::set_wrap_mode(glpp::Wrap wrap_mode)
{
}

void Texture::bind()
{
    glpp::bind_texture(_id);
}

void Texture::unbind()
{
    glpp::bind_texture<glpp::TextureKind::Tex2D>(0);
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