#include "compute_image_size.h"

namespace Cool {

auto compute_image_size(float aspect_ratio, float number_of_pixels) -> img::Size
{
    return {
        std::max(static_cast<img::Size::DataType>(std::round(std::sqrt(number_of_pixels * aspect_ratio))), static_cast<img::Size::DataType>(1)),
        std::max(static_cast<img::Size::DataType>(std::round(std::sqrt(number_of_pixels / aspect_ratio))), static_cast<img::Size::DataType>(1)),
    };
}

} // namespace Cool