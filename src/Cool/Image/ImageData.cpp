#include "ImageData.h"
#include <stb_image/stb_image.h>

namespace Cool {

ImageData::ImageData(std::string_view file_path, int nb_of_channels, bool flip_vertically)
    : nb_of_channels{nb_of_channels}
{
    stbi_set_flip_vertically_on_load(flip_vertically ? 1 : 0);
    int w, h; // NOLINT
    data.reset(stbi_load(file_path.data(), &w, &h, nullptr, nb_of_channels));
    if (!data || w < 1 || h < 1) {
        Log::error("[LoadImage::load] Couldn't load image from \"{}\":\n{}", file_path, stbi_failure_reason());
    }
    else {
        size.set_width(static_cast<ImageSize::DataType>(w));
        size.set_height(static_cast<ImageSize::DataType>(h));
    }
}

} // namespace Cool