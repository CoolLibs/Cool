#include "set_texture_from_opencv_image.h"

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

} // namespace Cool