#include "Texture.h"
#include <img/img.hpp>

namespace Cool {

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

Texture::Texture(img::Size const& size, uint8_t const* data, TextureLayout const& layout, TextureConfig config)
    : Texture{config}
{
    set_image(size, data, layout);
}

// static auto img_to_glpp_size(img::Size const& size) -> glpp::ImageSize
// {
//     return {static_cast<GLsizei>(size.width()), static_cast<GLsizei>(size.height())};
// }

void Texture::set_size(img::Size const& size)
{
    _aspect_ratio = img::SizeU::aspect_ratio(size);
    // _tex.resize(img_to_glpp_size(size));
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
            .internal_format = channels_count == 3 ? InternalFormat::RGB : InternalFormat::RGBA,
            .channels        = channels_count == 3 ? Channels::RGB : Channels::RGBA,
            .texel_data_type = TexelDataType::UnsignedByte,
        }
    );
}

void Texture::set_image(img::Size const& size, uint8_t const* data, TextureLayout const& layout)
{
    _aspect_ratio = img::SizeU::aspect_ratio(size);
    // _tex.upload_data(
    //     img_to_glpp_size(size),
    //     data,
    //     layout
    // );
#if DEBUG
    _data_has_been_uploaded = true;
#endif
}

void Texture::set_interpolation_mode(Interpolation interpolation_mode)
{
    bind();
    // _tex.set_minification_filter(interpolation_mode);
    // _tex.set_magnification_filter(interpolation_mode);
}

void Texture::set_wrap_mode(Wrap wrap_mode)
{
    bind();
    // _tex.set_wrap(wrap_mode);
}

void Texture::bind()
{
    // _tex.bind();
}

void Texture::unbind()
{
    // glpp::bind_texture<glpp::TextureKind::Tex2D>(0);
}

// void Texture::attach_to_slot(GLuint slot) const
// {
// #if DEBUG
//     if (!_data_has_been_uploaded)
//         Log::Debug::error("Texture::attach_to_slot", "You must upload some data (at least a width and height) before using the texture.");
// #endif
//     _tex.bind_to_texture_unit(slot);
// }

} // namespace Cool
