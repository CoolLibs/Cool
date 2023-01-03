#include "ColorSpace.h"

namespace Cool {

auto parse_color_space(std::string_view text) -> ColorSpace
{
    {
#include "generated/parse_color_space.inl"
    }

    throw std::runtime_error{fmt::format("Unable to parse color space from '{}'.", text)};
}

} // namespace Cool