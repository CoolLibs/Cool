#include "TextureSource_VideoFile.h"
#include "Cool/Video/hack_get_global_time_in_seconds.h"

namespace Cool {

auto TextureSource_VideoFile::imgui_widget() -> bool
{
    return _player->imgui_widget();
}

[[nodiscard]] auto TextureSource_VideoFile::get_texture() const -> Texture const*
{
    return _player->get_texture(hack_get_global_time_in_seconds());
}

auto TextureSource_VideoFile::get_error() const -> std::optional<std::string>
{
    return _player->get_error();
}

} // namespace Cool