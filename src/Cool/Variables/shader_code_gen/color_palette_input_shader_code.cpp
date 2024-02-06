#include "Cool/StrongTypes/ColorPalette.h"

namespace Cool {

auto color_palette_input_shader_code(Cool::ColorPalette const& value, std::string_view name) -> std::string
{
    // NB: we create a function rather than an array to hold our palette. That is because glsl doesn't allow arrays of size 0.
    using fmt::literals::operator""_a;
    return value.value.empty()
               ? fmt::format(
                   FMT_COMPILE(
                       R"STR(
const int {color_palette_function}_length = 0;

vec3 {color_palette_function}(int index)   
{{
    return vec3(0.);
}}
         )STR"
                   ),
                   "color_palette_function"_a = name
               )
               : fmt::format(
                   FMT_COMPILE(
                       R"STR( 
const int {color_palette_function}_length = {palette_size};
uniform vec3 {color_palette_name}[{palette_size}];

vec3 {color_palette_function}(int index) 
{{
    return {color_palette_name}[index];
}}
    )STR"
                   ),
                   "palette_size"_a           = value.value.size(),
                   "number_of_colors"_a       = fmt::format("number_of_colors_of_{}", internal::color_palette_array_name(name)),
                   "color_palette_function"_a = name,
                   "color_palette_name"_a     = internal::color_palette_array_name(name)
               );
}

} // namespace Cool