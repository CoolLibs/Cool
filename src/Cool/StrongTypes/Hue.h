#pragma once

#include <Cool/ImGui/ImGuiExtras.h>
#include <op/op.hpp>

namespace Cool {
struct Hue
    : public op::Addable<Hue>
    , public op::Subtractable<Hue>
    , public op::Negatable<Hue>
    , public op::EqualityComparable<Hue> {
    float value{};
    constexpr Hue() = default; // Constructors are not implcitly created by the compiler because we inherit from some stuff
    constexpr explicit Hue(float value)
        : value{value}
    {}
    auto get() const -> float { return from_0_to_1(); }
    void set(const float hue) { value = hue; }
    auto from_0_to_1() const -> float { return fmod(value, 1.f); }

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
    return std::to_string(hue.get());
}

inline auto hue_widget(const std::string& name, float& hue) -> bool
{
    return ImGuiExtras::hue_wheel(
        name.c_str(),
        &hue
    );
}

} // namespace Cool