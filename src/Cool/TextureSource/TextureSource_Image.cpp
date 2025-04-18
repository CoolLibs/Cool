#include "TextureSource_Image.h"
#include "Cool/Gpu/Texture.h"
#include "Cool/ImGui/ImGuiExtras.h"
#include "Cool/NfdFileFilter/NfdFileFilter.h"
#include "Cool/TextureSource/TextureLibrary_Image.h"

namespace Cool {

auto TextureSource_Image::imgui_widget() -> bool
{
    bool b = false;
    b |= ImGuiExtras::file_and_folder_opening("Image Path", &absolute_path, NfdFileFilter::ImageLoad);
    return b;
}

[[nodiscard]] auto TextureSource_Image::get_texture() const -> Texture const*
{
    return TextureLibrary_Image::instance().get(absolute_path);
}

auto TextureSource_Image::get_error() const -> std::optional<std::string>
{
    return TextureLibrary_Image::instance().error_from(absolute_path);
}

} // namespace Cool