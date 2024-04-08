#pragma once

namespace Cool {

enum class AlphaSpace {
#include "generated/alpha_space_enum.inl"
    Any, /// To indicate that the image can be interpreted as either space without any difference (i.e. the alpha is 1 everywhere)
};

/// Redefine == operator so that AlphaSpace::Any is equal to all alpha spaces
auto operator==(AlphaSpace a, AlphaSpace b) -> bool;

} // namespace Cool