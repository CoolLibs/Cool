#pragma once
#include <imgui.h>

namespace Cool::ImGuiExtras {

struct Style {
#include "generated_style/style_colors.inl"
#include "generated_style/style_declaration.inl"

    void imgui();

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
#include "generated_style/style_serialization.inl"
        );
    }
};

[[nodiscard]] inline auto GetStyle() -> Style&
{
    static auto instance = Style{};
    return instance;
}

}; // namespace Cool::ImGuiExtras