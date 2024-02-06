#pragma once
#include "Cool/Variables/glsl_type.h"
#include "Cool/Variables/shader_code_gen/shader_code_gen.h"

namespace Cool {

/// Defines the uniform in shader code; e.g. something like `uniform float name;` (or something more complex for some types like Gradient).
template<typename T> // TODO(Variables) Concept for all types T that are VariableTypes
auto gen_input_shader_code(T const& value, std::string_view name) -> std::string
{
    if constexpr (std::is_same_v<T, Cool::Gradient>)
        return gradient_input_shader_code(value, name);
    if constexpr (std::is_same_v<T, Cool::ColorPalette>)
        return color_palette_input_shader_code(value, name);
    if constexpr (std::is_same_v<T, Cool::MathExpression>)
        return math_expression_input_shader_code(value, name);
    return fmt::format("uniform {} {};", glsl_type<T>(), name);
}

} // namespace Cool