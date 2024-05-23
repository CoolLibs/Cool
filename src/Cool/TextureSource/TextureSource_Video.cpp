#include "TextureLibrary_Video.h"
#include "TextureSource_Video.h"


namespace Cool {

uint32_t TextureSource_Video::next_id{0};

auto TextureSource_Video::imgui_widget() -> bool
{
    return _video_descriptor.imgui_widget();
}

[[nodiscard]] auto TextureSource_Video::get_texture() const -> Texture const*
{
    return TextureLibrary_Video::instance().get({_id, _video_descriptor});
}

auto TextureSource_Video::get_error() const -> std::optional<std::string>
{
    return TextureLibrary_Video::instance().error_from({_id, _video_descriptor});
}

} // namespace Cool