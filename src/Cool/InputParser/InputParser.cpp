#include "InputParser.h"
#include <Cool/Dependencies/Input.h>
#include <Cool/String/String.h>
#include <type_from_string/type_from_string.hpp>

namespace Cool {

template<>
auto get_default_metadata(std::string_view key_values) -> Cool::VariableMetadata<Cool::RgbColor>
{
    Cool::VariableMetadata<Cool::RgbColor> metadata{};

    metadata.is_hdr = key_values.find("hdr") != std::string::npos;

    return metadata;
}

#include "Cool/Variables/generated/find_metadatas_in_string.inl"

auto make_any_input(
    std::string_view type,
    std::string_view name,
    DirtyFlag        dirty_flag,
    InputFactory_Ref input_factory,
    std::string_view key_values
) -> AnyInput
{
    return TFS_EVALUATE_FUNCTION_TEMPLATE(make_input, type, AnyInput, (name, dirty_flag, input_factory, key_values));
}

// TODO(LD) Move to String, and accept any first word, not just INPUT
auto is_commented_out(std::string_view line) -> bool
{
    const auto comment_pos = line.find("//");
    const auto input_pos   = line.find("INPUT");
    return comment_pos != std::string_view::npos &&
           input_pos != std::string_view::npos &&
           comment_pos < input_pos;
}

struct Type_and_name {
    std::string_view type;
    std::string_view name;
};

auto find_type_and_name(
    std::string_view line
) -> std::optional<Type_and_name>
{
    if (line.find("INPUT") == std::string::npos)
    {
        return std::nullopt;
    }
    const auto current_pos = line.find("INPUT") + 5;

    const auto type_position = Cool::String::find_next_word_position(line, current_pos);
    if (!type_position)
    {
        return std::nullopt;
    }
    const auto type = line.substr(type_position->first, type_position->second - type_position->first);

    // TODO(LD) Cool::String::substr(line, *type_position);

    const auto name = Cool::String::next_word(line, type_position->second);
    if (!name)
    {
        return std::nullopt;
    }

    return Type_and_name{*name, type};
}

auto find_key_values(
    std::string_view line
) -> std::optional<std::string_view>
{
    const auto comment_pos = line.find("//");
    if (comment_pos == std::string_view::npos)
    {
        return "";
    }
    const auto key_values_pos = Cool::String::find_next_word_position(line, comment_pos);
    if (!key_values_pos)
    {
        return std::nullopt;
    }
    return line.substr(key_values_pos->first, line.length() - key_values_pos->first);
}

auto try_parse_input(
    std::string_view line,
    DirtyFlag        dirty_flag,
    InputFactory_Ref input_factory
) -> std::optional<AnyInput>
{
    if (is_commented_out(line))
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

    const auto key_values = find_key_values(line);

    return make_any_input(type_and_name->type, type_and_name->name, dirty_flag, input_factory, *key_values);
}

auto get_inputs_from_shader_code(
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
            Cool::Log::ToUser::error("ShaderManager_FromText::parse_shader_for_params", "Error while parsing :\n{}", e.what());
        }
    }
    return new_inputs;
}

} // namespace Cool