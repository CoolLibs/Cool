#include "ImageSequence.hpp"
#include <cstddef>
#include "Cool/TextureSource/TextureLibrary_Image.h"

namespace Cool {

// TODO rerender when frame changes
// TODO lo  d images when opening project file
// TODO gui for framerate
// TODO handlle looping options (like video player)
// TODO improve perf
// TODO log error
// TODO move to its own texture source

auto ImageSequence::get_texture(Time const& time) const -> Texture const*
{
    int frame = static_cast<int>(time.as_seconds_float() * framerate);
    if (frame > static_cast<int>(_images_paths.size()) - 1 || frame < 0)
    {
        return nullptr;
    }
    else
    {
        return TextureLibrary_Image::instance().get(_images_paths[frame], 1s);
    }
}

void ImageSequence::set_folder(std::filesystem::path const& path)
{
    _folder_path = path;

    try
    {
        for (const auto& entry : std::filesystem::directory_iterator(_folder_path))
        {
            _images_paths.push_back(entry);
        }
    }
    catch (...)
    {
    }
}

} // namespace Cool