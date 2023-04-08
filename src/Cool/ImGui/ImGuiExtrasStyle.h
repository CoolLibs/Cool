#pragma once
#include <imgui.h>

namespace Cool::ImGuiExtras {

struct Style {
    ImVec4 toggle{};
    ImVec4 toggle_hovered{};
    ImVec4 toggle_bg{};
    ImVec4 toggle_bg_hovered{};
    ImVec4 highlight_items{};
};

[[nodiscard]] inline auto GetStyle() -> Style&
{
    static auto instance = Style{};
    return instance;
}

}; // namespace Cool::ImGuiExtras