#include "InputParser.h"
#include <Cool/Dependencies/Input.h>
#include <Cool/String/String.h>
#include <Cool/StrongTypes/RgbColor.h>
#include <Cool/Variables/Variable.h>
#include <sstream>
#include <type_from_string/type_from_string.hpp>

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
static auto get_default_metadata(std::string_view) -> Cool::VariableMetadata<T>
{
    static_assert(
#include <Cool/Variables/generated/T_is_a_variable_type.inl>
        , "No implementation found for this type! You can add it in generate_variables.py"
    );
    /// NB: Use the following code if you need to know the type which is failing:
    // const auto debug_name = std::string{"Type not supported yet: "} + typeid(T).name();
    // std::ignore           = debug_name;
    // assert(false);
    return Cool::VariableMetadata<T>{};
}

template<>
auto get_default_metadata(std::string_view key_values) -> Cool::VariableMetadata<Cool::RgbColor>
{
    Cool::VariableMetadata<Cool::RgbColor> metadata{};

    metadata.is_hdr = key_values.find("hdr") != std::string::npos;

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
    return TFS_EVALUATE_FUNCTION_TEMPLATE(make_input, type, AnyInput, (name, dirty_flag, input_factory, key_values));
}

struct TypeAndName_Ref {
    std::string_view type{};
    std::string_view name{};
};

/// /!\ The returned string_views are only valid as long as the input string_view is valid!
static auto find_type_and_name(std::string_view line)
    -> std::optional<TypeAndName_Ref>
{
    static constexpr auto input_keyword = std::string_view{"INPUT"};
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
    if (Cool::String::is_commented_out(line))
    {
        return std::nullopt;
    }

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