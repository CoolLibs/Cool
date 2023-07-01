#pragma once
#include "glpp/glpp.hpp"

namespace Cool {

enum class TextureRepeatMode {
    None,
    Mirror,
    Mosaic,
    Clamp
};

struct TextureSamplerDescriptor {
    TextureRepeatMode   repeat_mode{TextureRepeatMode::None};
    glpp::Interpolation interpolation_mode{glpp::Interpolation::Linear};

    friend auto operator<=>(TextureSamplerDescriptor const&, TextureSamplerDescriptor const&) = default;
};

} // namespace Cool