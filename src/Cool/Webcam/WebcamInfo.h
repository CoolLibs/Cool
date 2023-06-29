#pragma once
#include <compare>
#include "Cool/Gpu/TextureSamplerInfo.h"
#include "Cool/Path/Path.h"

namespace Cool {

struct WebcamInfo {
    // std::filesystem::path absolute_path{Cool::Path::default_texture()};
    int                webcam_index{};
    TextureSamplerInfo sampler{};

    auto        imgui_widget(const int number_of_webcam) -> bool;
    friend auto operator==(WebcamInfo const& a, WebcamInfo const& b) -> bool = default;
    friend auto operator<=>(WebcamInfo const& a, WebcamInfo const& b)
    {
        // operator<=> implemented manually because MacOS's standard library doesn't have it yet for std::filesystem::path :(

        {
            auto const order = a.sampler <=> b.sampler;
            if (order != std::strong_ordering::equal)
                return order;
        }
        {
            int const order = a.webcam_index < b.webcam_index;
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
            cereal::make_nvp("Path", webcam_index),
            cereal::make_nvp("Repeat Mode", sampler.repeat_mode),
            cereal::make_nvp("Interpolation Mode", sampler.interpolation_mode)
        );
    }
};

} // namespace Cool