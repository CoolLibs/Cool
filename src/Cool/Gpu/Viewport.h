#pragma once

#include <Cool/Image/ImageSize.h>

namespace Cool {

struct Viewport {
    ImageSize size;
    glm::vec2 bottom_left_corner;
};

} // namespace Cool