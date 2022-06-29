#pragma once

#include <math.h>
#include <op/op.hpp>
#include "Angle.h"

inline auto get_angle_from_vector(const glm::vec2& direction) -> Cool::Angle
{
    return Cool::Angle(Cool::Radians(atan2(direction.y, direction.x)));
}

namespace Cool {
struct Direction2D : public op::Addable<Direction2D>
    , public op::Subtractable<Direction2D>
    , public op::Negatable<Direction2D>
    , public op::EqualityComparable<Direction2D> {
    glm::vec2   value{};
    Cool::Angle angle{};
    constexpr Direction2D() = default;
    explicit Direction2D(glm::vec2 value)
        : value{normalize(value)}, angle{get_angle_from_vector(value)}
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
    return "(" + std::to_string(direction.value.x) + "," + std::to_string(direction.value.y) + ")";
}
} // namespace Cool

// inline auto get_angle_from_direction(Cool::Direction2D direction) -> Cool::Angle
// {
//     return Cool::Angle(Cool::Radians(atan2(direction.value.y, direction.value.x)));
// }