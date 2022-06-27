#pragma once

#include <math.h>
#include <op/op.hpp>
#include "Angle.h"

namespace Cool {
struct Direction2D : public op::Addable<Direction2D>
    , public op::Subtractable<Direction2D>
    , public op::Negatable<Direction2D>
    , public op::Multipliable<Direction2D>
    , public op::Scalable<Direction2D>
    , public op::EqualityComparable<Direction2D> {
    Cool::Angle value;
    constexpr Direction2D() = default;
    constexpr explicit Direction2D(glm::vec2 vector)
        : value{Cool::Angle(Cool::Radians(atan2(vector.y, vector.x)))}
    {}
    constexpr explicit Direction2D(Cool::Angle angle)
        : value{angle}
    {}

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Direction", value));
    }
};

inline auto to_string(Cool::Direction2D direction) -> std::string
{
    return to_string(direction.value);
}
} // namespace Cool