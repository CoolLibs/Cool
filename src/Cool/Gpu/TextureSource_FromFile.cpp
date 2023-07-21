#pragma once
#include "TextureSource_FromFile.h"
#include <compare>
#include "Cool/Gpu/Texture.h"
#include "Cool/Gpu/TextureLibrary_FromFile.h"
#include "Cool/ImGui/ImGuiExtras.h"
#include "Cool/NfdFileFilter/NfdFileFilter.h"
#include "Cool/Path/Path.h"

namespace Cool {

auto TextureSource_FromFile::imgui_widget() -> bool
{
    bool b = false;
    b |= ImGuiExtras::file_and_folder("Image Path", &absolute_path, NfdFileFilter::Image);
    return b;
}

[[nodiscard]] auto TextureSource_FromFile::get_texture() const -> std::optional<Texture> const&
{
    return TextureLibrary_FromFile::instance().get(absolute_path);

} // TODO(TD) à remettre

auto TextureSource_FromFile::get_error() const -> std::optional<std::string>
{
    return TextureLibrary_FromFile::instance().error_from(absolute_path);
}

} // namespace Cool