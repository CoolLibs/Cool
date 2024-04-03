#include "TextureSource_FromFile.h"
#include "Cool/Gpu/TextureLibrary_FromFile.h"
#include "Cool/ImGui/ImGuiExtras.h"
#include "Cool/NfdFileFilter/NfdFileFilter.h"
#include "Cool/WebGPU/Texture.h"

namespace Cool {

auto TextureSource_FromFile::imgui_widget() -> bool
{
    bool b = false;
    b |= ImGuiExtras::file_and_folder("Image Path", &absolute_path, NfdFileFilter::Image);
    return b;
}

[[nodiscard]] auto TextureSource_FromFile::get_texture() const -> Texture const*
{
    return TextureLibrary_FromFile::instance().get(absolute_path);
}

auto TextureSource_FromFile::get_error() const -> std::optional<std::string>
{
    return TextureLibrary_FromFile::instance().error_from(absolute_path);
}

} // namespace Cool