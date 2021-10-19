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