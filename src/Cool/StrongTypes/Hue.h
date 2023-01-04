#pragma once

#include <Cool/ImGui/ImGuiExtras.h>
#include <op/op.hpp>

namespace Cool {

struct Hue
    : public op::Addable<Hue>
    , public op::Subtractable<Hue>
    , public op::Negatable<Hue> {
    constexpr Hue() = default; // Constructors are not implicitly created by the compiler because we inherit from some stuff
    /// Hues are numbers from 0 to 1. 0 and 1 correspond to red.
    explicit Hue(float hue)
        : value{std::fmod(hue, 1.f)}
    {}

    friend auto operator==(const Hue& a, const Hue& b) -> bool { return a.value == b.value; }
    friend auto operator!=(const Hue& a, const Hue& b) -> bool { return !(a == b); }

    /// Hues are numbers from 0 to 1. 0 and 1 correspond to red.
    auto from_0_to_1() const -> float { return value; }
    /// Hues are numbers from 0 to 1. 0 and 1 correspond to red.
    void set(float hue) { value = std::fmod(hue, 1.f); }

    auto imgui_widget(std::string_view name) -> bool
    {
        return ImGuiExtras::hue_wheel(
            name.data(),
            &value
        );
    }

private:
    float value{};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    auto save_minimal(Archive const&) const -> float
    {
        return value;
    }
    template<class Archive>
    void load_minimal(Archive const&, float const& val)
    {
        value = val;
    }
};

inline auto to_string(Cool::Hue hue) -> std::string
{
    return std::to_string(hue.from_0_to_1());
}

inline auto imgui_widget(std::string_view name, Cool::Hue& hue) -> bool
{
    return hue.imgui_widget(name);
}

} // namespace Cool