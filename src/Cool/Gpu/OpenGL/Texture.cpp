#include <glpp-extended/src/TextureLayout.h>
#if defined(COOL_OPENGL)

#include <img/img.hpp>
#include "../Texture.h"
#include "glpp-extended/src/ImageSize.h"
#include "glpp/Enums/Channels.h"
#include "glpp/Enums/InternalFormat.h"
#include "glpp/Enums/TexelDataType.h"
#include "glpp/Enums/TextureKind.h"
#include "glpp/Functions/Texture.h"
#include "img/src/Size.h"
#include "img/src/SizeU.h"

namespace Cool::OpenGL {

Texture::Texture(TextureConfig config)
{
    set_wrap_mode(config.wrap_mode);
    set_interpolation_mode(config.interpolation_mode);
}

Texture::Texture(img::Size const& size, TextureConfig config)
    : Texture{config}
{
    set_size(size);
}

Texture::Texture(img::Image const& image, TextureConfig config)
    : Texture{config}
{
    set_image(image);
}

Texture::Texture(img::Size const& size, int channels_count, uint8_t const* data, TextureConfig config)
    : Texture{config}
{
    set_image(size, channels_count, data);
}

static auto img_to_glpp_size(img::Size const& size) -> glpp::ImageSize
{
    return {static_cast<GLsizei>(size.width()), static_cast<GLsizei>(size.height())};
}

void Texture::set_size(img::Size const& size)
{
    _aspect_ratio = img::SizeU::aspect_ratio(size);
    _tex.resize(img_to_glpp_size(size));
#if DEBUG
    _data_has_been_uploaded = true;
#endif
}

void Texture::set_image(img::Image const& img)
{
    set_image(img.size(), img.channels_count(), img.data());
}

void Texture::set_image(img::Size const& size, int channels_count, uint8_t const* data)
{
    assert(channels_count == 3 || channels_count == 4);
    set_image(
        size,
        data,
        {
            .internal_format = channels_count == 3 ? glpp::InternalFormat::RGB : glpp::InternalFormat::RGBA,
            .channels        = channels_count == 3 ? glpp::Channels::RGB : glpp::Channels::RGBA,
            .texel_data_type = glpp::TexelDataType::UnsignedByte,
        }
    );
}

void Texture::set_image(img::Size const& size, uint8_t const* data, glpp::TextureLayout const& layout)
{
    _aspect_ratio = img::SizeU::aspect_ratio(size);
    _tex.upload_data(
        img_to_glpp_size(size),
        data,
        layout
    );
#if DEBUG
    _data_has_been_uploaded = true;
#endif
}

void Texture::set_interpolation_mode(glpp::Interpolation interpolation_mode)
{
    bind();
    glpp::set_minification_filter<glpp::TextureKind::Tex2D>(*_tex, interpolation_mode);
    glpp::set_magnification_filter<glpp::TextureKind::Tex2D>(*_tex, interpolation_mode);
}

void Texture::set_wrap_mode(glpp::Wrap wrap_mode)
{
    bind();
    glpp::set_wrap<glpp::TextureKind::Tex2D>(*_tex, wrap_mode);
}

void Texture::bind()
{
    _tex.bind();
}

void Texture::unbind()
{
    glpp::bind_texture<glpp::TextureKind::Tex2D>(0);
}

void Texture::attach_to_slot(GLuint slot) const
{
#if DEBUG
    if (!_data_has_been_uploaded)
        Log::Debug::error("Texture::attach_to_slot", "You must upload some data (at least a width and height) before using the texture.");
#endif
    _tex.bind_to_texture_unit(slot);
}

} // namespace Cool::OpenGL

#endif