#pragma once
#include <compare>
#include "Cool/Path/Path.h"
#include "TextureSamplerInfo.h"

namespace Cool {

struct TextureInfo {
    std::filesystem::path absolute_path{Cool::Path::root() / "res/images/logo mask.png"};
    TextureSamplerInfo    sampler{};

    auto        imgui_widget() -> bool;
    friend auto operator==(TextureInfo const& a, TextureInfo const& b) -> bool = default;
    friend auto operator<=>(TextureInfo const& a, TextureInfo const& b)
    {
        // operator<=> implemented manually because MacOS's standard library doesn't have it yet for std::filesystem::path :(

        {
            auto const order = a.sampler <=> b.sampler;
            if (order != std::strong_ordering::equal)
                return order;
        }
        {
            int const order = strcmp(a.absolute_path.string().c_str(), b.absolute_path.string().c_str());
            if (order < 0)
                return std::strong_ordering::less;
            if (order > 0)
                return std::strong_ordering::greater;
            return std::strong_ordering::equal;
        }
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