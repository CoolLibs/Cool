#include "TextureLibrary_Webcam.hpp"

namespace Cool {

auto TextureLibrary_Webcam::instance() -> TextureLibrary_Webcam&
{
    static auto inst = TextureLibrary_Webcam{};
    return inst;
}

void TextureLibrary_Webcam::keep_image_alive_this_frame(std::shared_ptr<wcam::Image const> const& image)
{
    _images_to_keep_alive_for_this_frame.push_back(image);
}

void TextureLibrary_Webcam::on_frame_end()
{
    _images_to_keep_alive_for_this_frame.clear();
}

} // namespace Cool