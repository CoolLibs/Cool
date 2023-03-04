#pragma once
#include "TextureSamplerInfo.h"

namespace Cool {

struct TextureInfo {
    std::filesystem::path absolute_path{};
    TextureSamplerInfo    sampler{};

    auto        imgui_widget() -> bool;
    friend auto operator<=>(TextureInfo const&, TextureInfo const&) = default;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Path", absolute_path),
            cereal::make_nvp("Repeat Mode", sampler.repeat_mode),
            cereal::make_nvp("Interpolation Mode", sampler.interpolation_mode)
        );
    }
};

} // namespace Cool