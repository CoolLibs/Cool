#pragma once
#include <imgui.h>

namespace Cool::ImGuiExtras {

struct Style {
    ImVec4 toggle{};
    ImVec4 toggle_hovered{};
    ImVec4 toggle_bg{};
    ImVec4 toggle_bg_hovered{};
    ImVec4 highlight_items{};
    ImVec4 gradient_mark{};
    ImVec4 gradient_mark_hovered{};
    ImVec4 gradient_mark_selected{};
    ImVec4 gradient_mark_selected_hovered{};
};

[[nodiscard]] inline auto GetStyle() -> Style&
{
    static auto instance = Style{};
    return instance;
}

}; // namespace Cool::ImGuiExtras