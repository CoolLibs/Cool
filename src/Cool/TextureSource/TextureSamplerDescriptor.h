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
    auto        imgui_widget() -> bool;

private:
    // Serialization
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            ser20::make_nvp("Repeat Mode", repeat_mode),
            ser20::make_nvp("Interpolation Mode", interpolation_mode)
        );
    }
};

} // namespace Cool