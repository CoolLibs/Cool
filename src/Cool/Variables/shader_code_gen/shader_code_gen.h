#pragma once
#include "Cool/StrongTypes/ColorPalette.h"
#include "Cool/StrongTypes/Gradient.h"
#include "Cool/StrongTypes/MathExpression.h"

namespace Cool {

auto color_palette_input_shader_code(Cool::ColorPalette const& value, std::string_view name) -> std::string;
auto gradient_input_shader_code(Cool::Gradient const& value, std::string_view name) -> std::string;
auto math_expression_input_shader_code(Cool::MathExpression const& value, std::string_view name) -> std::string;

} // namespace Cool