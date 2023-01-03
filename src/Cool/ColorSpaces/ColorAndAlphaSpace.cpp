#include "ColorAndAlphaSpace.h"

namespace Cool {

auto parse_color_and_alpha_space(std::string_view text) -> ColorAndAlphaSpace
{
    {
#include "generated/parse_color_and_alpha_space.inl"
    }

    throw std::runtime_error{fmt::format("Unable to parse color and alpha space from '{}'.", text)};
}

} // namespace Cool