#include "gen_input_shader_code.h"
#include <Cool/Variables/glsl_type.h>
#include "Cool/Variables/shader_code_gen/shader_code_gen.h"

namespace Cool {

template<typename T>
static auto gen_input_shader_code_impl(T const& value, std::string_view name) -> std::string
{
    if constexpr (std::is_same_v<T, Cool::Gradient>)
        return gradient_input_shader_code(value, name);
    if constexpr (std::is_same_v<T, Cool::ColorPalette>)
        return color_palette_input_shader_code(value, name);
    if constexpr (std::is_same_v<T, Cool::MathExpression>)
        return math_expression_input_shader_code(value, name);
    return fmt::format("uniform {} {};", glsl_type<T>(), name);
}

auto gen_input_shader_code(AnyInput const& input) -> std::string
{
    return std::visit([&](auto&& input) { return gen_input_shader_code_impl(input.value(), input.name()); }, input);
}

auto gen_input_shader_code(AnyInput const& input, std::string_view name) -> std::string
{
    return std::visit([&](auto&& input) { return gen_input_shader_code_impl(input.value(), name); }, input);
}

} // namespace Cool