#pragma once
#include <imgui.h>

namespace Cool::ImGuiExtras {

struct Style {
#include "generated_style/style_colors.inl"

    float floating_buttons_spacing = 4.f;

    auto imgui() -> bool;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Floating buttons spacing", floating_buttons_spacing)
        );
    }
};

[[nodiscard]] inline auto GetStyle() -> Style&
{
    static auto instance = Style{};
    return instance;
}

}; // namespace Cool::ImGuiExtras