#include "../ImageData.h"
#include <stb_image/stb_image.h>

namespace Cool {

ImageData::ImageData(std::string_view file_path, int nb_of_channels, bool flip_vertically)
    : nb_of_channels{nb_of_channels}
{
    stbi_set_flip_vertically_on_load(flip_vertically ? 1 : 0);
    int w, h;
    data.reset(stbi_load(file_path.data(), &w, &h, nullptr, nb_of_channels));
    width  = static_cast<uint32_t>(w);
    height = static_cast<uint32_t>(h);
    if (!data) {
        Log::error("[LoadImage::load] Couldn't open file \"{}\"", file_path);
    }
}

} // namespace Cool