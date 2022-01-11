#pragma once

#include "internal/Parameter_Color.h"
#include "internal/Parameter_Dir3.h"
#include "internal/Parameter_Float.h"
#include "internal/Parameter_Int.h"
#include "internal/Parameter_Rot3.h"
#include "internal/Parameter_Vec2.h"
#include "internal/Parameter_Vec3.h"

namespace Cool::Parameter {

using Any = std::variant<
    Parameter::Int,
    Parameter::Float,
    Parameter::Vec2,
    Parameter::Vec3,
    Parameter::Dir3,
    Parameter::Rot3,
    Parameter::Color>;

using AnyDesc = std::variant<
    Parameter::IntDesc,
    Parameter::FloatDesc,
    Parameter::Vec2Desc,
    Parameter::Vec3Desc,
    Parameter::Dir3Desc,
    Parameter::Rot3Desc,
    Parameter::ColorDesc>;

} // namespace Cool::Parameter

namespace Cool {

inline Parameter::Any make_any_parameter(const Parameter::AnyDesc& desc)
{
    return std::visit([](auto&& desc) -> Parameter::Any {
        using T = std::decay_t<decltype(desc)>;
        if constexpr (std::is_same_v<T, Parameter::IntDesc>) {
            return Parameter::Int{desc};
        }
        else if constexpr (std::is_same_v<T, Parameter::FloatDesc>) {
            return Parameter::Float{desc};
        }
        else if constexpr (std::is_same_v<T, Parameter::Vec2Desc>) {
            return Parameter::Vec2{desc};
        }
        else if constexpr (std::is_same_v<T, Parameter::Vec3Desc>) {
            return Parameter::Vec3{desc};
        }
        else if constexpr (std::is_same_v<T, Parameter::Dir3Desc>) {
            return Parameter::Dir3{desc};
        }
        else if constexpr (std::is_same_v<T, Parameter::ColorDesc>) {
            return Parameter::Color{desc};
        }
        else if constexpr (std::is_same_v<T, Parameter::Rot3Desc>) {
            return Parameter::Rot3{desc};
        }
    },
                      desc);
}

template<typename T>
concept ParameterType = requires(T param)
{
    // clang-format off
    typename T::Description;
    // typename T::InternalValue;
    // {desc.name} -> std::convertible_to<std::string>;
    // {desc.default_value} -> std::convertible_to<typename T::InternalValue>;
    // clang-format on
};

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