#include "TextureSource_VideoFile.h"
#include "TextureLibrary_VideoFile.h"

namespace Cool {

uint32_t TextureSource_VideoFile::next_id{0};

auto TextureSource_VideoFile::imgui_widget() -> bool
{
    return _video_descriptor.imgui_widget();
}

[[nodiscard]] auto TextureSource_VideoFile::get_texture() const -> Texture const*
{
    return TextureLibrary_VideoFile::instance().get({_id, _video_descriptor});
}

auto TextureSource_VideoFile::get_error() const -> std::optional<std::string>
{
    return TextureLibrary_VideoFile::instance().error_from({_id, _video_descriptor});
}

} // namespace Cool