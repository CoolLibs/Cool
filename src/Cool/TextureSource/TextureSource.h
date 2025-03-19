#pragma once
#include "Cool/Gpu/Texture.h"
#include "TextureSource_Image.h"
#include "TextureSource_ImageSequence.hpp"
#include "TextureSource_Video.h"
#include "TextureSource_Webcam.h"

namespace Cool {

using TextureSource = std::variant<
    TextureSource_Image,
    TextureSource_Webcam,
    TextureSource_Video,
    TextureSource_ImageSequence
    // when adding a new, must be at the end otherwise breaks serialization
    >;

auto get_texture(TextureSource const&) -> Texture const&;
auto imgui_widget(TextureSource&) -> bool;
auto get_error(TextureSource const&) -> std::optional<std::string>;

} // namespace Cool