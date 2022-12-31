#pragma once

namespace Cool {

enum class ColorSpace {
#include "generated/color_space_enum.inl"
};

auto parse_color_space(std::string_view) -> ColorSpace;

} // namespace Cool