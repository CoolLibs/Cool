#include "InputParser.h"
#include <Cool/Dependencies/Input.h>
#include <Cool/String/String.h>
#include <Cool/StrongTypes/RgbColor.h>
#include <Cool/Variables/Variable.h>
#include <Cool/Variables/glsl_type.h>
#include <Cool/type_from_string/type_from_string.h>
#include <fmt/compile.h>
#include <sstream>

namespace Cool {

template<typename T>
static auto get_default_value(std::string_view key_values) -> T
{
    const auto default_T = Cool::String::find_value_for_given_key<T>(key_values, "default");

    if (default_T)
    {
        return *default_T;
    }

    // `default` keyword is allowed to be omitted
    const auto default_without_key = Cool::String::next_word(key_values, 0);
    if (default_without_key)
    {
        const auto default_value = Cool::String::value_from_string<T>(*default_without_key);
        if (default_value)
        {
            return *default_value;
        }
    }

    return T{};
}

template<typename T>
static auto get_default_metadata(std::string_view) -> Cool::VariableMetadata<T>;

template<>
auto get_default_metadata(std::string_view key_values) -> Cool::VariableMetadata<Cool::RgbColor>
{
    Cool::VariableMetadata<Cool::RgbColor> metadata{};

    metadata.is_hdr = Cool::String::contains_word("hdr", key_values);

    return metadata;
}

#include <Cool/Variables/generated/find_metadatas_in_string.inl>

template<typename T>
static auto make_input(
    const std::string_view            name,
    const std::optional<std::string>& description,
    DirtyFlag                         dirty_flag,
    InputFactory_Ref                  input_factory,
    std::string_view                  key_values
) -> Input<T>
{
    return input_factory.make<T>(
        dirty_flag,
        name,
        description,
        get_default_value<T>(key_values),
        get_default_metadata<T>(key_values)
    );
}

static auto make_any_input(
    std::string_view                  type,
    std::string_view                  name,
    const std::optional<std::string>& description,
    DirtyFlag                         dirty_flag,
    InputFactory_Ref                  input_factory,
    std::string_view                  key_values
) -> AnyInput
{
    return COOL_TFS_EVALUATE_FUNCTION_TEMPLATE(make_input, type, AnyInput, (name, description, dirty_flag, input_factory, key_values));
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

/// Returns the string containing all the key<->values associations.
/// That is, the string that is after the // on a line
/// /!\ The returned string_views are only valid as long as the input string_view is valid!
static auto find_key_values(
    std::string_view line
) -> std::string_view
{
    const auto comment_pos = line.find("//");
    if (comment_pos == std::string_view::npos)
    {
        return "";
    }
    const auto key_values_start = comment_pos + 2; // +2 to skip the two slashes
    return Cool::String::substring(line, key_values_start, line.length());
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
    InputFactory_Ref input_factory
) -> std::optional<AnyInput>
{
    const auto type_and_name = find_type_and_name(line);
    if (!type_and_name)
    {
        return std::nullopt;
    }

    // TODO(JF) TODO(LD) Make this a reality
    // const std::vector<std::pair<std::string, std::string>> replacements = {
    //     std::make_pair("_", " "),
    // };
    // Cool::String::replace_all(name, "_", " ");

    return make_any_input(
        type_and_name->type,
        type_and_name->name,
        parse_description(line),
        dirty_flag,
        input_factory,
        find_key_values(line)
    );
}

auto parse_all_inputs(
    std::string_view source_code,
    DirtyFlag        dirty_flag,
    InputFactory_Ref input_factory
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
            const auto input = try_parse_input(line, dirty_flag, input_factory);
            if (input)
            {
                new_inputs.emplace_back(std::move(*input));
            }
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

template<typename T>
auto instantiate_shader_code__impl(const T&, std::string_view name) -> std::string
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
    using namespace fmt::literals;
    switch (interpolation_mode)
    {
    case ImGG::Interpolation::Linear:
    {
        return fmt::format(
            FMT_COMPILE(R"STR(
            float mix_factor = (x_wrapped - {gradient_marks}[i - 1].pos) /
                            ({gradient_marks}[i].pos - {gradient_marks}[i - 1].pos);
            return mix({gradient_marks}[i - 1].col, {gradient_marks}[i].col, mix_factor);
    )STR"),
            "gradient_marks"_a = fmt::format("{}_", name)
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
    return fmt::format("number_of_marks_of_{}", internal::gradient_marks_array_name(name));
}

template<>
auto instantiate_shader_code__impl(const Cool::Gradient& value, std::string_view name) -> std::string
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
// #include "_COOL_RES_/shaders/GradientMark.glsl"
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
auto instantiate_shader_code__impl(const Cool::ColorPalette& value, std::string_view name) -> std::string
{
    // NB: we create a fnuction rather than an array to hold our calette. That is because glsl doesn't allow arrays of size 0.
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

template<typename T>
auto instantiate_shader_code(const Input<T>& input, Cool::InputProvider_Ref input_provider) -> std::string
{
    return instantiate_shader_code__impl(input_provider(input), input.name());
}

static auto instantiate_shader_code(std::string_view name, const std::vector<AnyInput>& inputs, Cool::InputProvider_Ref input_provider) -> std::string
{
    std::string res;
    for (const auto& input : inputs)
    {
        std::visit(
            [&](auto&& input) {
                if (input.name() == name)
                {
                    res = instantiate_shader_code(input, input_provider);
                }
            },
            input
        );
    }
    return res;
}

auto preprocess_inputs(std::string_view source_code, const std::vector<AnyInput>& inputs, Cool::InputProvider_Ref input_provider) -> std::string
{
    std::stringstream in{std::string{source_code}};
    std::stringstream out{};
    std::string       line;
    while (getline(in, line))
    {
        if (const auto info = find_type_and_name(line))
        {
            out << instantiate_shader_code(info->name, inputs, input_provider) << '\n';
        }
        else
        {
            out << line << '\n';
        }
    }
    return out.str();
}

} // namespace Cool

#if COOL_ENABLE_TESTS
#include "doctest/doctest.h"

// TODO(LD) TODO(JF) Move this in a Cool/Testing/testing.h (and same for the print of vector)
namespace doctest {
template<typename T>
doctest::String toString(const std::optional<T>& value)
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

TEST_CASE("find_key_values()")
{
    CHECK(Cool::find_key_values("//") == "");
    CHECK(Cool::find_key_values("// ") == " ");
    CHECK(Cool::find_key_values("// Hello") == " Hello");
    CHECK(Cool::find_key_values("/ Hello") == "");
    CHECK(Cool::find_key_values("Hello") == "");
    CHECK(Cool::find_key_values("Hello //") == "");
    CHECK(Cool::find_key_values("Hello // ") == " ");
    CHECK(Cool::find_key_values("Hello // World") == " World");
}

TEST_CASE("Parsing a RgbColor")
{
    {
        const auto color_metadata = Cool::get_default_metadata<Cool::RgbColor>("// hdr");
        CHECK(
            color_metadata.is_hdr == true
        );
    }
    {
        const auto color_metadata = Cool::get_default_metadata<Cool::RgbColor>("// nohdr");
        CHECK(
            color_metadata.is_hdr == false
        );
    }
    {
        const auto color_metadata = Cool::get_default_metadata<Cool::RgbColor>("// ");
        CHECK(
            color_metadata.is_hdr == false
        );
    }
}

TEST_CASE("parse_description()")
{
    CHECK(Cool::parse_description("// Hello") == std::nullopt);
    CHECK(Cool::parse_description("/// Hello") == "Hello");
    CHECK(Cool::parse_description("///Hello") == "Hello");
    CHECK(Cool::parse_description("///") == "");
    CHECK(Cool::parse_description("/// ") == "");
}

#endif