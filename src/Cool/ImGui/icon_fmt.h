#pragma once
#include "IcoMoonCodepoints.h" // This will be useful to anyone including this file

namespace Cool {

/// Formats a text by appending an icon in front of it.
auto icon_fmt(std::string_view text, std::string_view icon, bool only_when_extra_icons = false) -> std::string;

} // namespace Cool