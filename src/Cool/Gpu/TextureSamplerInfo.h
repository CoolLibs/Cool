#pragma once
#include "glpp/glpp.hpp"

namespace Cool {

enum class TextureRepeatMode {
    None,
    Mirror,
    Mosaic,
    Clamp
};

struct TextureSamplerInfo {
    TextureRepeatMode   repeat_mode{TextureRepeatMode::None};
    glpp::Interpolation interpolation_mode{glpp::Interpolation::Linear};

    friend auto operator<=>(TextureSamplerInfo const&, TextureSamplerInfo const&) = default;
};

} // namespace Cool