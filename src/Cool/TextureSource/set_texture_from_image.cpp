#include "set_texture_from_image.h"
#include "easy_ffmpeg/easy_ffmpeg.hpp"
#include "opencv2/opencv.hpp"

namespace Cool {

void set_texture_from_opencv_image(std::optional<Texture>& texture, cv::Mat const& image)
{
    auto const size = img::Size{
        static_cast<unsigned int>(image.cols),
        static_cast<unsigned int>(image.rows),
    };
    auto const layout = glpp::TextureLayout{
        .internal_format = glpp::InternalFormat::RGBA,
        .channels        = glpp::Channels::BGR, // TODO(Video) Some video files might be BGRA (and we have a problem when importing gifs (e.g. Animation.gif), which might be caused by that too)
        .texel_data_type = glpp::TexelDataType::UnsignedByte,
    };
    auto const* data = static_cast<uint8_t const*>(image.ptr());

    if (!texture)
    {
        texture = Texture{size, data, layout};
    }
    else
    {
        texture->bind(); // TODO(Video) API texture->while_bound(callback), that automatically rebinds the texture that was previously bound
        texture->set_image(size, data, layout);
        Cool::Texture::unbind();
    }
}

void set_texture_from_ffmpeg_image(std::optional<Texture>& texture, ffmpeg::Frame const& image)
{
    if (!image.is_different_from_previous_frame && texture.has_value())
        return;

    auto const size = img::Size{
        static_cast<unsigned int>(image.width),
        static_cast<unsigned int>(image.height),
    };
    auto const layout = glpp::TextureLayout{
        .internal_format = glpp::InternalFormat::RGBA,
        .channels        = glpp::Channels::RGBA,
        .texel_data_type = glpp::TexelDataType::UnsignedByte,
    };
    uint8_t const* const data = image.data;

    if (!texture)
    {
        texture = Texture{size, data, layout};
    }
    else
    {
        texture->bind(); // TODO(Video) API texture->while_bound(callback), that automatically rebinds the texture that was previously bound
        texture->set_image(size, data, layout);
        Cool::Texture::unbind();
    }
}

} // namespace Cool