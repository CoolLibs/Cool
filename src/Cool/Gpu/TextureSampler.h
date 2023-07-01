#pragma once
#include "glpp/glpp.hpp"

namespace Cool {

enum class TextureRepeatMode {
    None,
    Mirror,
    Mosaic,
    Clamp
};

struct TextureSampler {
    TextureRepeatMode   repeat_mode{TextureRepeatMode::None};
    glpp::Interpolation interpolation_mode{glpp::Interpolation::Linear};

    friend auto operator<=>(TextureSampler const&, TextureSampler const&) = default;
};

} // namespace Cool