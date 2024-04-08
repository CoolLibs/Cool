#include "AlphaSpace.h"

namespace Cool {

auto operator==(AlphaSpace a, AlphaSpace b) -> bool
{
    if (static_cast<int>(a) == static_cast<int>(AlphaSpace::Any)
        || static_cast<int>(b) == static_cast<int>(AlphaSpace::Any))
        return true;
    return static_cast<int>(a) == static_cast<int>(b);
}

} // namespace Cool

#if COOL_ENABLE_TESTS
#include <doctest/doctest.h>
TEST_CASE("AlphaSpace::Any is equal to all alpha spaces")
{
    CHECK(Cool::AlphaSpace::Straight == Cool::AlphaSpace::Any);
    CHECK(Cool::AlphaSpace::Premultiplied == Cool::AlphaSpace::Any);
    CHECK(Cool::AlphaSpace::Any == Cool::AlphaSpace::Any);
    CHECK(Cool::AlphaSpace::Any == Cool::AlphaSpace::Straight);
    CHECK(Cool::AlphaSpace::Any == Cool::AlphaSpace::Premultiplied);

    CHECK(Cool::AlphaSpace::Straight != Cool::AlphaSpace::Premultiplied);
    CHECK(Cool::AlphaSpace::Premultiplied != Cool::AlphaSpace::Straight);
}
#endif