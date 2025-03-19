#include "TextureSource_ImageSequence.hpp"
#include "Cool/Gpu/Texture.h"
#include "Cool/ImGui/ImGuiExtras.h"
#include "Cool/NfdFileFilter/NfdFileFilter.h"
#include "Cool/TextureSource/TextureLibrary_Image.h"
#include "Cool/Video/hack_get_global_time_in_seconds.h"

namespace Cool {

auto TextureSource_ImageSequence::imgui_widget() -> bool
{
    return image_sequence.imgui();
}

[[nodiscard]] auto TextureSource_ImageSequence::get_texture() const -> Texture const*
{
    return image_sequence.get_texture(hack_get_global_time_in_seconds());
}

auto TextureSource_ImageSequence::get_error() const -> std::optional<std::string>
{
    return {};
    // return TextureLibrary_Image::instance().error_from(absolute_path);
}

} // namespace Cool