#pragma once
#include <img/src/Size.h>

namespace Cool {

auto compute_image_size(float aspect_ratio, float number_of_pixels) -> img::Size;

} // namespace Cool