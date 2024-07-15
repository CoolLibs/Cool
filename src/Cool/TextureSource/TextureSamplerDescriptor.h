#pragma once

namespace Cool {

enum class TextureRepeatMode {
    None,
    Mirror,
    Mosaic,
    Clamp
};

enum class Interpolation {
    NearestNeighbour,
    Linear,
    LinearMipmapLinear,
};

struct TextureSamplerDescriptor {
    TextureRepeatMode repeat_mode{TextureRepeatMode::None};
    Interpolation     interpolation_mode{Interpolation::Linear};

    friend auto operator<=>(TextureSamplerDescriptor const&, TextureSamplerDescriptor const&) = default;
    auto        imgui_widget() -> bool;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Repeat Mode", repeat_mode),
            cereal::make_nvp("Interpolation Mode", interpolation_mode)
        );
    }
};

} // namespace Cool