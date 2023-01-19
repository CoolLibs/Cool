#pragma once
#include "Cool/StrongTypes/Color.h"

namespace Cool {

class ScopedTitleBarColor { // NOLINT(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
public:
    explicit ScopedTitleBarColor(Cool::Color const& color);
    ~ScopedTitleBarColor();
};

} // namespace Cool