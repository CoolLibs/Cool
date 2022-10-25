#pragma once

#include <img/img.hpp>

namespace Cool::ImageSizeU {

enum class WH {
    None,
    Width,
    Height,
};

auto imgui(img::Size& size) -> WH; // Returns which side was modified (or WH::None if the widget was not used)

} // namespace Cool::ImageSizeU