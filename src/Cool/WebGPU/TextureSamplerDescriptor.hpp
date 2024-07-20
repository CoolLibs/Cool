#pragma once
#include "InterpolationMode.hpp"
#include "RepeatMode.hpp"

namespace Cool {

struct TextureSamplerDescriptor {
    RepeatMode        repeat_mode{RepeatMode::None};
    InterpolationMode interpolation_mode{InterpolationMode::Linear};

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