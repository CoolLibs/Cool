#pragma once

namespace Cool {

enum class ColorAndAlphaSpace {
#include "generated/color_and_alpha_space_enum.inl"
};

auto parse_color_and_alpha_space(std::string_view) -> ColorAndAlphaSpace;

} // namespace Cool