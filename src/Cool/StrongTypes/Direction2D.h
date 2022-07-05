#pragma once

#include <cmath>
#include <op/op.hpp>
#include "Angle.h"

namespace Cool {
inline auto constexpr get_angle_from_vector(const glm::vec2& direction) -> Cool::Angle
{
    return Cool::Angle{Cool::Radians{
        std::atan2(direction.x, direction.y)}};
}
class Direction2D
    : public op::Negatable<Direction2D>
    , public op::EqualityComparable<Direction2D> {
public:
    void rotate(const Cool::Angle angle) { value += angle; };

    auto as_unit_vec2() const -> glm::vec2
    {
        return glm::vec2{glm::cos(value.value.value), glm::sin(value.value.value)};
    }

    auto as_angle() const -> Cool::Angle { return value; };

    void set_direction_from_angle(const Cool::Angle& angle) { value = angle; };
    void set_direction_from_vec2(const glm::vec2& vector) { value = get_angle_from_vector(vector); };

    constexpr Direction2D() = default;
    explicit constexpr Direction2D(glm::vec2 value)
        : value{get_angle_from_vector(value)}
    {}

    explicit constexpr Direction2D(Angle angle)
        : value{angle}
    {}

public:
    Cool::Angle value{};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Angle", value));
    }
};

inline auto to_string(Cool::Direction2D direction) -> std::string
{
    return glm::to_string(direction.as_unit_vec2());
}
} // namespace Cool
