#pragma once

#include <op/op.hpp>

namespace Cool {
struct Hue
    : public op::Addable<Hue>
    , public op::Subtractable<Hue>
    , public op::Negatable<Radians>
    , public op::EqualityComparable<Hue> {
    float value{};
    constexpr Hue() = default; // Constructors are not implcitly created by the compiler because we inherit from some stuff
    constexpr explicit Hue(float value)
        : value{value}
    {}

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Hue", value));
    }
};

inline auto to_string(Cool::Hue hue) -> std::string
{
    return std::to_string(hue.value);
}

} // namespace Cool