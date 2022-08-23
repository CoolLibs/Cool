#include "InputParser.h"
#include <Cool/Dependencies/Input.h>
#include <Cool/Dependencies/InputProvider_Ref.h>
#include <Cool/String/String.h>
#include <Cool/StrongTypes/RgbColor.h>
#include <Cool/Variables/Variable.h>
#include <Cool/Variables/glsl_type.h>
#include <Cool/type_from_string/type_from_string.h>
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
    const std::string_view name,
    DirtyFlag              dirty_flag,
    InputFactory_Ref       input_factory,
    std::string_view       key_values
) -> Input<T>
{
    return input_factory.make<T>(
        dirty_flag,
        name,
        get_default_value<T>(key_values),
        get_default_metadata<T>(key_values)
    );
}

static auto make_any_input(
    std::string_view type,
    std::string_view name,
    DirtyFlag        dirty_flag,
    InputFactory_Ref input_factory,
    std::string_view key_values
) -> AnyInput
{
    return COOL_TFS_EVALUATE_FUNCTION_TEMPLATE(make_input, type, AnyInput, (name, dirty_flag, input_factory, key_values));
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
        dirty_flag,
        input_factory,
        find_key_values(line)
    );
}

auto parse_all_inputs(
    std::string_view source_code,
    DirtyFlag        dirty_flag,
    InputFactory_Ref input_factory
) -> std::vector<AnyInput>
{
    std::vector<AnyInput> new_inputs;
    std::stringstream     stream{std::string{source_code}};
    std::string           line;
    while (getline(stream, line))
    {
        try
        {
            const auto input = try_parse_input(line, dirty_flag, input_factory);
            if (input)
            {
                new_inputs.push_back(*input);
            }
        }
        catch (const std::exception& e)
        {
            Cool::Log::ToUser::error(
                "InputParser::parse_all_inputs",
                fmt::format("Error while parsing:\n{}", e.what())
            );
        }
    }
    return new_inputs;
}

template<typename T>
auto instantiate_shader_code__value(const T& value, std::string_view name) -> std::string
{
    return fmt::format("uniform {} {};", glsl_type<T>(), name);
}

template<>
auto instantiate_shader_code__value(const Cool::Gradient& value, std::string_view name) -> std::string
{
    return fmt::format(
        R"STR(
// #include "_ROOT_FOLDER_/res/shader-examples/gradient/Mark.glsl"
const int number_of_marks = 3;
Mark gradient_marks[number_of_marks] = Mark[](

    Mark(0.0f, vec4(0.4933167749070636f, 0.16508965227506256f, 0.704906923613742f, 1.f)),

    Mark(0.5f, vec4(0.09816811100558287f, 0.31570548361171435f, 0.9383085172073401f, 1.f)),

    Mark(1.f, vec4(0.3674398336005895f, 0.6947963741375294f, 0.28958490820456584f, 1.f))
);
vec4 {}(float x)
{{
    if (x <= gradient_marks[0].pos)
    {{
        return gradient_marks[0].col;
    }}
    for (int i = 1; i < number_of_marks; i++)
    {{
        if ((x <= gradient_marks[i].pos) && (x >= gradient_marks[i - 1].pos))
        {{
            float mix_factor = (x - gradient_marks[i - 1].pos) /
                            (gradient_marks[i].pos - gradient_marks[i - 1].pos);
            return mix(gradient_marks[i - 1].col, gradient_marks[i].col, mix_factor);
        }}
    }}
    if (x >= gradient_marks[number_of_marks - 1].pos)
    {{
        return gradient_marks[number_of_marks - 1].col;
    }}
}}
    )STR",
        // declare_all_marks(value),
        name
    );
}

template<typename T>
auto instantiate_shader_code__input(const Input<T>& input, Cool::InputProvider_Ref input_provider) -> std::string
{
    return instantiate_shader_code__value(input_provider(input), input.name());
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
                    res = instantiate_shader_code__input(input, input_provider);
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
            //             if (info->type == "Gradient")
            //             {
            //                 out << fmt::format(
            //                     R"STRING(
            // vec4 {}(float x)
            // {{
            //     return vec4(x);
            // }}
            //                 )STRING",
            //                     info->name
            //                 );
            //             }
            //             else
            //             {
            //                 out << fmt::format("uniform {} {};\n", info->type, info->name);
            //             }
        }
        else
        {
            out << line << '\n';
        }
    }
    return out.str();
}

} // namespace Cool

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