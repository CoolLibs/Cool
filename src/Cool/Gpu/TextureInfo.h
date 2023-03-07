#pragma once
#include <compare>
#include "TextureSamplerInfo.h"

namespace Cool {

struct TextureInfo {
    std::filesystem::path absolute_path{};
    TextureSamplerInfo    sampler{};

    auto        imgui_widget() -> bool;
    friend auto operator==(TextureInfo const& a, TextureInfo const& b) -> bool = default;
    friend auto operator<=>(TextureInfo const& a, TextureInfo const& b)
    {
        // operator<=> implemented manually because MacOS's standard library doesn't have it yet for std::filesystem::path :(
        auto const order = a.sampler <=> b.sampler;
        if (order != std::strong_ordering::equal)
            return order;
        return a.absolute_path.string() <=> b.absolute_path.string();
    }

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