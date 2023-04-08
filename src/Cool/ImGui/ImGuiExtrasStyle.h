#pragma once
#include <imgui.h>

namespace Cool::ImGuiExtras {

struct Style {
#include "generated_style/style_struct.inl"
};

[[nodiscard]] inline auto GetStyle() -> Style&
{
    static auto instance = Style{};
    return instance;
}

}; // namespace Cool::ImGuiExtras