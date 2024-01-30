#include "InputParser.h"
#include <Cool/Dependencies/Input.h>
#include <Cool/String/String.h>
#include <Cool/StrongTypes/Color.h>
#include <Cool/Variables/Variable.h>
#include <Cool/Variables/glsl_type.h>
#include <Cool/type_from_string/type_from_string.h>
#include <sstream>
#include <string_view>
#include "Cool/ColorSpaces/ColorAndAlphaSpace.h"
#include "Cool/ColorSpaces/ColorSpace.h"
#include "Cool/StrongTypes/ColorAndAlpha.h"
#include "Cool/StrongTypes/MathExpression.h"
#include "fmt/format.h"

namespace Cool {

template<typename T>
static auto make_input(
    const std::string_view            name,
    const std::optional<std::string>& description,
    DirtyFlag                         dirty_flag,
    DirtyFlag                         secondary_dirty_flag,
    std::string_view                  type
) -> Input<T>
{
    auto input = Input<T>{
        Variable<T>{{std::string{name}}},
        dirty_flag,
        description,
        secondary_dirty_flag
    };

    if constexpr (std::is_same_v<T, Cool::Color>)
    {
        input._desired_color_space = static_cast<int>(parse_color_space(type));
    }
    else if constexpr (std::is_same_v<T, Cool::ColorAndAlpha>)
    {
        input._desired_color_space = static_cast<int>(parse_color_and_alpha_space(type));
    }

    return input;
}

static auto make_any_input(
    std::string_view                  type,
    std::string_view                  name,
    const std::optional<std::string>& description,
    DirtyFlag                         dirty_flag,
    DirtyFlag                         secondary_dirty_flag
) -> AnyInput
{
    return AnyInput{COOL_TFS_EVALUATE_FUNCTION_TEMPLATE(make_input, type, AnyInput, (name, description, dirty_flag, secondary_dirty_flag, type))};
}

struct TypeAndName_Ref {
    std::string_view type{};
    std::string_view name{};
};

/// /!\ The returned string_views are only valid as long as the input string_view is valid!
static auto find_type_and_name(std::string_view line)
    -> std::optional<TypeAndName_Ref>
{
    if (Cool::String::is_commented_out(line))
    {
        return std::nullopt;
    }

    static constexpr auto input_keyword = "INPUT"sv;
    const auto            input_pos     = line.find(input_keyword);
    if (input_pos == std::string_view::npos)
    {
        return std::nullopt;
    }
    const auto current_pos = input_pos + input_keyword.length();

    const auto type_position = Cool::String::find_next_word_position(line, current_pos);
    if (!type_position)
    {
        return std::nullopt;
    }

    const auto name = Cool::String::next_word(line, type_position->second);
    if (!name)
    {
        return std::nullopt;
    }

    return TypeAndName_Ref{
        .type = Cool::String::substring(line, *type_position),
        .name = *name,
    };
}

static auto parse_description(std::string_view line) -> std::optional<std::string>
{
    auto pos = line.find("///");
    if (pos == std::string_view::npos)
        return {};
    pos += 3; // Skip the ///

    if (pos < line.size() && std::isspace(line[pos])) // Skip first space
        pos += 1;
    return std::string{Cool::String::substring(line, pos, line.size())};
}

auto try_parse_input(
    std::string_view line,
    DirtyFlag        dirty_flag,
    DirtyFlag        secondary_dirty_flag
) -> std::optional<AnyInput>
{
    const auto type_and_name = find_type_and_name(line);
    if (!type_and_name)
    {
        return std::nullopt;
    }

    return make_any_input(
        type_and_name->type,
        type_and_name->name,
        parse_description(line),
        dirty_flag,
        secondary_dirty_flag
    );
}

auto parse_all_inputs(
    std::string_view source_code,
    DirtyFlag        dirty_flag,
    DirtyFlag        secondary_dirty_flag
) -> tl::expected<std::vector<AnyInput>, std::string>
{
    std::vector<AnyInput> new_inputs;
    std::stringstream     stream{std::string{source_code}};
    std::string           line;
    size_t                line_count = 0;
    while (getline(stream, line))
    {
        line_count++;
        try
        {
            auto input = try_parse_input(line, dirty_flag, secondary_dirty_flag);
            if (input)
                new_inputs.emplace_back(std::move(*input));
        }
        catch (const std::exception& e)
        {
            return tl::make_unexpected(
                fmt::format("Invalid INPUT on line {}:\n{}", line_count, e.what())
            );
        }
    }
    return new_inputs;
}
// TODO(Variables) Move shader code gen in another file ?
template<typename T>
static auto gen_input_shader_code__impl(const T&, std::string_view name) -> std::string
{
    return fmt::format("uniform {} {};", glsl_type<T>(), name);
}

static auto gen_code__wrap_mode(ImGG::WrapMode wrap_mode) -> std::string
{
    switch (wrap_mode)
    {
    case ImGG::WrapMode::Clamp:
    {
        return "clamp(x, 0., 1.);";
    }
    case ImGG::WrapMode::Repeat:
    {
        return "fract(x);";
    }
    case ImGG::WrapMode::MirrorRepeat:
    {
        return "1. - abs(mod(x, 2.) -1.);";
    }
    default:
    {
        Cool::Log::Debug::error("InputParser::gen_code__wrap_mode", "Invalid WrapMode enum value");
        return "";
    }
    }
}

static auto gen_code__interpolation(std::string_view name, ImGG::Interpolation interpolation_mode) -> std::string
{
    using fmt::literals::operator""_a;
    switch (interpolation_mode)
    {
    case ImGG::Interpolation::Linear_Light:
    case ImGG::Interpolation::Linear_Paint:
    {
        return fmt::format(
            FMT_COMPILE(R"STR(
            // The color is in Oklab space with premultiplied alpha because interpolating in that space looks better (it matches human perception).
            float mix_factor = (x_wrapped - {gradient_marks}[i - 1].pos) /
                            ({gradient_marks}[i].pos - {gradient_marks}[i - 1].pos);
            vec4 col1 = {gradient_marks}[i - 1].col;
            vec4 col2 = {gradient_marks}[i    ].col;
            return {mix_colors};
    )STR"),
            "gradient_marks"_a = fmt::format("{}_", name),
            "mix_colors"_a     = interpolation_mode == ImGG::Interpolation::Linear_Light
                                     ? "mix(col1, col2, mix_factor)"
                                     : R"STR(
        vec4(
            Cool_premultiply_color(
                Cool_Oklab_from_sRGB(mixbox_lerp(
                    Cool_sRGB_from_Oklab(Cool_unpremultiply_color(col1.rgb, col1.a)),
                    Cool_sRGB_from_Oklab(Cool_unpremultiply_color(col2.rgb, col2.a)),
                    mix_factor
                )),
                mix(col1.a, col2.a, mix_factor)
            ),
            mix(col1.a, col2.a, mix_factor)
        )
                )STR"
        );
    }
    case ImGG::Interpolation::Constant:
    {
        return fmt::format(
            FMT_COMPILE(R"STR(
            return {gradient_marks}[i].col;
    )STR"),
            "gradient_marks"_a = fmt::format("{}_", name)
        );
    }
    default:
    {
        Cool::Log::Debug::error("InputParser::gen_code__interpolation", "Invalid Interpolation enum value");
        return "";
    }
    }
}

static auto gen_code__number_of_marks_variable_name(std::string_view name)
{
    return fmt::format("numberOfMarksOf{}", internal::gradient_marks_array_name(name));
}

template<>
auto gen_input_shader_code__impl(Cool::Gradient const& value, std::string_view name) -> std::string
{
    using namespace fmt::literals;
    return value.value.gradient().is_empty()
               ? fmt::format(
                   R"STR(
vec4 {}(float x)   
{{
    return vec4(0.);
}}
         )STR",
                   name
               )
               : fmt::format(
                   FMT_COMPILE(R"STR( 
#include "_COOL_RES_/shaders/GradientMark.glsl"
const int {number_of_marks} = {gradient_size};
uniform GradientMark {gradient_marks}[{number_of_marks}];

vec4 {gradient_function}(float x) // we benchmarked and linear scan is faster that dichotomy
{{
    float x_wrapped = {wrap};
    if (x_wrapped <= {gradient_marks}[0].pos)
    {{
        return {gradient_marks}[0].col;
    }}
    for (int i = 1; i < {number_of_marks}; i++)
    {{
        if ((x_wrapped <= {gradient_marks}[i].pos) && (x_wrapped >= {gradient_marks}[i - 1].pos))
        {{
            {interpolation}
        }}
    }}
    if (x_wrapped >= {gradient_marks}[{number_of_marks} - 1].pos)
    {{
        return {gradient_marks}[{number_of_marks} - 1].col;
    }}
}}
    )STR"),
                   "gradient_size"_a     = value.value.gradient().get_marks().size(),
                   "number_of_marks"_a   = gen_code__number_of_marks_variable_name(name),
                   "gradient_function"_a = name,
                   "wrap"_a              = gen_code__wrap_mode(value.wrap_mode),
                   "interpolation"_a     = gen_code__interpolation(name, value.value.gradient().interpolation_mode()),
                   "gradient_marks"_a    = internal::gradient_marks_array_name(name)
               );
}

static auto gen_code__number_of_colors_variable_name(std::string_view name)
{
    return fmt::format("number_of_colors_of_{}", internal::color_palette_array_name(name));
}

template<>
auto gen_input_shader_code__impl(const Cool::ColorPalette& value, std::string_view name) -> std::string
{
    // NB: we create a function rather than an array to hold our palette. That is because glsl doesn't allow arrays of size 0.
    using namespace fmt::literals;
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
                   "number_of_colors"_a       = gen_code__number_of_colors_variable_name(name),
                   "color_palette_function"_a = name,
                   "color_palette_name"_a     = internal::color_palette_array_name(name)
               );
}

template<>
auto gen_input_shader_code__impl(Cool::MathExpression const& expression, std::string_view name) -> std::string
{
    using namespace fmt::literals;
    return fmt::format(
        FMT_COMPILE(
            R"STR(
float {name}(float x)
{{
    return {expression};
}};
            )STR"
        ),
        "expression"_a = expression.expression,
        "name"_a       = name
    );
}

template<typename T>
static auto gen_input_shader_code(Input<T> const& input) -> std::string
{
    return gen_input_shader_code<T>(input, input.name());
}

template<typename T>
static auto gen_input_shader_code(Input<T> const& input, std::string_view name /* Allows us to use a different name than the input's user-facing name if we want to */) -> std::string // TODO(Variables) Is this overload where we overwrite the name still useful?
{
    return gen_input_shader_code__impl(input.value(), name);
}

auto gen_input_shader_code(AnyInput const& input) -> std::string
{
    return std::visit([&](auto&& input) { return gen_input_shader_code(input); }, input);
}

auto gen_input_shader_code(AnyInput const& input, std::string_view name) -> std::string
{
    return std::visit([&](auto&& input) { return gen_input_shader_code(input, name); }, input);
}

} // namespace Cool

#if COOL_ENABLE_TESTS
#include "doctest/doctest.h"

// TODO(LD) TODO(JF) Move this in a Cool/Testing/testing.h (and same for the print of vector)
namespace doctest {
template<typename T>
doctest::String toString(std::optional<T> const& value)
{
    if (value)
    {
        return "Some(" + toString(*value) + ")";
    }
    else
    {
        return "None";
    }
}
} // namespace doctest

#endif