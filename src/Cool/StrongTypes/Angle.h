#pragma once

#include <numbers>
#include <op/op.hpp>

namespace Cool {

constexpr float TAU = std::numbers::pi_v<float> * 2.f;

struct Radians
    : public op::Addable<Radians>
    , public op::Subtractable<Radians>
    , public op::Negatable<Radians>
    , public op::Scalable<Radians>
    , public op::EqualityComparable<Radians> {
    float value{0.f};
    constexpr Radians() = default;

    constexpr explicit Radians(float value)
        : value{value} {}

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Radians", value));
    }
};

constexpr Radians turns_to_radians(float turns)
{
    return Radians{turns * TAU};
}

constexpr Radians degrees_to_radians(float degrees)
{
    return Radians{degrees / 360.f * TAU};
}

inline float radians_to_degrees(Radians radians)
{
    return radians.value / TAU * 360.f;
}

inline float radians_to_turns(Radians radians)
{
    return radians.value / TAU;
}

class Angle
    : public op::Addable<Angle>
    , public op::Subtractable<Angle>
    , public op::Negatable<Angle>
    , public op::Scalable<Angle>
    , public op::EqualityComparable<Angle> {
public:
    constexpr Angle() = default;
    constexpr explicit Angle(Radians value)
        : value{value} {}

    float as_turns() const { return radians_to_turns(value); }
    float as_radians() const { return value.value; }
    float as_degrees() const { return radians_to_degrees(value); }

    Radians value{};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Radians", value));
    }
};

inline auto to_string(Cool::Angle angle) -> std::string
{
    return std::to_string(angle.as_turns()) + " turn";
}

} // namespace Cool

constexpr Cool::Angle operator""_turn(long double turns)
{
    return Cool::Angle{Cool::turns_to_radians(static_cast<float>(turns))};
}

constexpr Cool::Angle operator""_degrees(long double degrees)
{
    return Cool::Angle{Cool::degrees_to_radians(static_cast<float>(degrees))};
}

constexpr Cool::Angle operator""_radians(long double radians)
{
    return Cool::Angle{Cool::Radians{static_cast<float>(radians)}};
}

constexpr Cool::Angle operator""_turn(unsigned long long int turns)
{
    return operator""_turn(static_cast<long double>(turns));
}

constexpr Cool::Angle operator""_degrees(unsigned long long int degrees)
{
    return operator""_degrees(static_cast<long double>(degrees));
}

constexpr Cool::Angle operator""_radians(unsigned long long int radians)
{
    return operator""_radians(static_cast<long double>(radians));
}