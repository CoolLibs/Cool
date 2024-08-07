#pragma once

#include <Cool/ImGui/ImGuiExtras.h>
#include <Cool/Math/constants.h>
#include <op/op.hpp>

namespace Cool {

struct Radians
    : public op::Addable<Radians>
    , public op::Subtractable<Radians>
    , public op::Negatable<Radians>
    , public op::Scalable<Radians> {
    float value{0.f};

    constexpr Radians() = default; // Constructors are not implicitly created by the compiler because we inherit from some stuff

    constexpr explicit Radians(float radians)
        : value{radians} {}

    friend auto operator==(Radians const& a, Radians const& b) -> bool { return a.value == b.value; }

private:
    // Serialization
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(ser20::make_nvp("Radians", value));
    }
};

constexpr auto turns_to_radians(float turns) -> Radians
{
    return Radians{turns * tau};
}

constexpr auto degrees_to_radians(float degrees) -> Radians
{
    return Radians{degrees / 360.f * tau};
}

inline auto radians_to_degrees(Radians radians) -> float
{
    return radians.value / tau * 360.f;
}

inline auto radians_to_turns(Radians radians) -> float
{
    return radians.value / tau;
}

class Angle
    : public op::Addable<Angle>
    , public op::Subtractable<Angle>
    , public op::Negatable<Angle> {
public:
    constexpr Angle() = default;     // Constructors are not implicitly created by the compiler because we inherit from some stuff
    constexpr Angle(Radians radians) // NOLINT(*explicit-constructor, *explicit-conversions)
        : value{radians}
    {}

    auto as_turns() const -> float { return radians_to_turns(value); }
    auto as_radians() const -> float { return value.value; }
    auto as_degrees() const -> float { return radians_to_degrees(value); }

    auto imgui_widget(std::string_view name, int number_of_snaps, float snaps_offset, bool always_snap) -> bool;

    friend auto operator==(Angle const& a, Angle const& b) -> bool { return a.value == b.value; }

private:
    friend class op::Addable<Angle>;
    Radians value{};

private:
    // Serialization
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(ser20::make_nvp("Radians", value));
    }
};

inline auto to_string(Cool::Angle angle) -> std::string
{
    return std::to_string(angle.as_turns()) + " turn";
}

inline auto imgui_widget(std::string_view name, Cool::Angle& angle, int number_of_snaps, float snaps_offset, bool always_snap) -> bool
{
    return angle.imgui_widget(name, number_of_snaps, snaps_offset, always_snap);
}

} // namespace Cool

constexpr auto operator""_turn(long double turns) -> Cool::Angle
{
    return Cool::Angle{Cool::turns_to_radians(static_cast<float>(turns))};
}

constexpr auto operator""_degrees(long double degrees) -> Cool::Angle
{
    return Cool::Angle{Cool::degrees_to_radians(static_cast<float>(degrees))};
}

constexpr auto operator""_radians(long double radians) -> Cool::Angle
{
    return Cool::Angle{Cool::Radians{static_cast<float>(radians)}};
}

constexpr auto operator""_turn(unsigned long long int turns) -> Cool::Angle
{
    return operator""_turn(static_cast<long double>(turns));
}

constexpr auto operator""_degrees(unsigned long long int degrees) -> Cool::Angle
{
    return operator""_degrees(static_cast<long double>(degrees));
}

constexpr auto operator""_radians(unsigned long long int radians) -> Cool::Angle
{
    return operator""_radians(static_cast<long double>(radians));
}