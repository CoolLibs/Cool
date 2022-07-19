#include "InputParser.h"
#include <Cool/Dependencies/Input.h>
#include <Cool/String/String.h>
#include <type_from_string/type_from_string.hpp>

namespace Cool {

template<>
auto get_default_metadata(std::string_view key_values) -> Cool::VariableMetadata<Cool::RgbColor>
{
    Cool::VariableMetadata<Cool::RgbColor> metadata{};

    if (key_values.find("hdr") != std::string::npos)
    {
        metadata.is_hdr = true;
    }
    else
    {
        metadata.is_hdr = false;
    }
    return metadata;
}

#include "Cool/Variables/generated/find_metadatas_in_string.inl"

auto make_any_input(
    const std::string_view type,
    const std::string_view name,
    DirtyFlag              dirty_flag,
    InputFactory_Ref       input_factory,
    std::string_view       key_values
) -> AnyInput
{
    return TFS_EVALUATE_FUNCTION_TEMPLATE(make_input, type, AnyInput, (name, dirty_flag, input_factory, key_values));
}

auto is_input_declaration(std::string line) -> bool
{
    return line.find("input") != std::string::npos;
}

auto is_commented_out(std::string line) -> bool
{
    const auto comment_pos = line.find("//");
    const auto input_pos   = line.find("input");
    if (comment_pos != std::string::npos &&
        input_pos != std::string::npos)
    {
        if (comment_pos < input_pos)
        {
            return true;
        }
    }
    return false;
}

auto try_parse_input(
    std::string      line,
    DirtyFlag        dirty_flag,
    InputFactory_Ref input_factory
) -> std::optional<AnyInput>
{
    if (!is_input_declaration(line) ||
        is_commented_out(line))
    {
        return std::nullopt;
    }

    auto current_pos = Cool::String::find_next_word_position(line, 0);
    if (!current_pos)
    {
        return std::nullopt;
    }

    const auto type_position = Cool::String::find_next_word_position(line, current_pos->second);
    if (type_position)
    {
        current_pos->second = type_position->second; // find_next_word_position to find current_pos even if there are whitespaces between words
    }
    else
    {
        return std::nullopt;
    }
    const auto type = line.substr(type_position->first, type_position->second - type_position->first);

    const auto name = Cool::String::next_word(line, current_pos->second);
    if (!name)
    {
        return std::nullopt;
    }

    // TODO(JF) TODO(LD) Make this a reality
    // const std::vector<std::pair<std::string, std::string>> replacements = {
    //     std::make_pair("_", " "),
    // };
    // Cool::String::replace_all(name, "_", " ");

    const auto comment_pos = line.find("//");
    if (comment_pos == std::string_view::npos)
    {
        return make_any_input(type, *name, dirty_flag, input_factory, "");
    }
    const auto key_values_pos = Cool::String::find_next_word_position(line, comment_pos);
    if (!key_values_pos)
    {
        return std::nullopt;
    }
    const auto key_values = line.substr(key_values_pos->first, line.length() - key_values_pos->first);

    return make_any_input(type, *name, dirty_flag, input_factory, key_values);
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