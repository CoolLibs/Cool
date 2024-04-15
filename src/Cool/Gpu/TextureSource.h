#pragma once
#include "Cool/Gpu/Texture.h"
#include "TextureSource_FromFile.h"
#include "TextureSource_FromWebcam.h"
#include "TextureSource_VideoFile.h"

namespace Cool {

using TextureSource = std::variant<
    TextureSource_FromFile,
    TextureSource_FromWebcam,
    TextureSource_VideoFile>;

auto get_texture(TextureSource const&) -> Texture const&;
auto imgui_widget(TextureSource&) -> bool;
auto get_error(TextureSource const&) -> std::optional<std::string>;

} // namespace Cool