#pragma once

#include <img/img.hpp>

namespace Cool {

struct Viewport {
    img::Size  size;
    glm::ivec2 bottom_left_corner;
};

} // namespace Cool