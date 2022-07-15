#pragma once

#include <cmath>
#include <op/op.hpp>
#include "Angle.h"

namespace Cool {

/// `direction` doesn't need to be normalized
inline auto constexpr get_angle_from_vector(glm::vec2 direction) -> Cool::Angle
{
    return Cool::Angle{Cool::Radians{
        std::atan2(
            direction.y,
            direction.x
        )}};
}

class Direction2D
    : public op::Negatable<Direction2D> {
public:
    constexpr Direction2D() = default;
    explicit constexpr Direction2D(glm::vec2 value)
        : value{get_angle_from_vector(value)}
    {}

    explicit constexpr Direction2D(Angle angle)
        : value{angle}
    {}

    friend auto operator==(const Direction2D& a, const Direction2D& b) -> bool { return a.value == b.value; }
    friend auto operator!=(const Direction2D& a, const Direction2D& b) -> bool { return !(a == b); }

    auto as_unit_vec2() const -> glm::vec2
    {
        return glm::vec2{
            glm::cos(value.value.value),
            glm::sin(value.value.value)};
    }

    auto as_angle() const -> Cool::Angle
    {
        return value;
    };

    void rotate(Cool::Angle angle) { value += angle; };
    void set(Cool::Angle angle) { value = angle; };
    void set(glm::vec2 vector) { value = get_angle_from_vector(vector); };

private:
    friend class op::Negatable<Direction2D>;
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
    return to_string(direction.as_angle());
}

} // namespace Cool
