#pragma once
#include <imgui.h>

namespace Cool::ImGuiExtras {

struct Style {
#include "generated_style/style_colors.inl"
#include "generated_style/style_declaration.inl"
#include "generated_style/style_imgui_declarations.inl"
};

[[nodiscard]] inline auto GetStyle() -> Style&
{
    static auto instance = Style{};
    return instance;
}

}; // namespace Cool::ImGuiExtras