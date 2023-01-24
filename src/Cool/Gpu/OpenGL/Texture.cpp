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

void Texture::set_size(img::Size const& size)
{
    _tex.resize({static_cast<GLsizei>(size.width()), static_cast<GLsizei>(size.height())});
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
    _tex.bind();
}

void Texture::unbind()
{
    glpp::bind_texture<glpp::TextureKind::Tex2D>(0);
}

void Texture::attach_to_slot(int slot) const
{
#if DEBUG
    if (!_data_has_been_uploaded)
        Log::Debug::error("Texture::attach_to_slot", "You must upload some data (at least a width and height) before using the texture.");
#endif
    _tex.bind_to_texture_unit(slot);
}

} // namespace Cool::OpenGL

#endif