#pragma once

#include "internal/Parameter_Color.h"
#include "internal/Parameter_Dir3.h"
#include "internal/Parameter_Float.h"
#include "internal/Parameter_Int.h"
#include "internal/Parameter_Vec2.h"

namespace Cool::Parameter {

using Any = std::variant<
    Parameter::Int,
    Parameter::Float,
    Parameter::Vec2,
    Parameter::Dir3,
    Parameter::Color>;

using AnyDesc = std::variant<
    Parameter::IntDesc,
    Parameter::FloatDesc,
    Parameter::Vec2Desc,
    Parameter::Dir3Desc,
    Parameter::ColorDesc>;

} // namespace Cool::Parameter

namespace Cool {

inline Parameter::Any make_any_parameter(const Parameter::AnyDesc& desc)
{
    return std::visit([](auto&& desc) {
        using T = std::decay_t<decltype(desc)>;
        if constexpr (std::is_same_v<T, Parameter::IntDesc>) {
            return Parameter::Any{Parameter::Int{desc}};
        }
        else if constexpr (std::is_same_v<T, Parameter::FloatDesc>) {
            return Parameter::Any{Parameter::Float{desc}};
        }
        else if constexpr (std::is_same_v<T, Parameter::Vec2Desc>) {
            return Parameter::Any{Parameter::Vec2{desc}};
        }
        else if constexpr (std::is_same_v<T, Parameter::Dir3Desc>) {
            return Parameter::Any{Parameter::Dir3{desc}};
        }
        else if constexpr (std::is_same_v<T, Parameter::ColorDesc>) {
            return Parameter::Any{Parameter::Color{desc}};
        }
        else {
            static_assert(false, "non-exhaustive visitor!");
        }
    },
                      desc);
}

template<typename T>
concept ParameterDesc = requires(T desc)
{
    // clang-format off
    typename T::Value;
    typename T::InternalValue;
    {desc.name} -> std::convertible_to<std::string>;
    {desc.default_value} -> std::convertible_to<typename T::InternalValue>;
    // clang-format on
};

} // namespace Cool