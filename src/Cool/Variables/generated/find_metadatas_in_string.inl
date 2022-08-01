/* -----------------------------------------------------------------------------
 * This file was automatically generated by a Python script.
 * PLEASE DON'T EDIT IT DIRECTLY, your changes would be overwritten the next time the script is run.
 * Instead, go to "generate_variables.py" and edit the "find_metadatas_in_string" function there.
 * -----------------------------------------------------------------------------
 */

template<>
auto get_default_metadata(std::string_view) -> Cool::VariableMetadata<bool>
{
    Cool::VariableMetadata<bool> metadata{};
    return metadata;
}

template<>
auto get_default_metadata(std::string_view key_values) -> Cool::VariableMetadata<int>
{
    Cool::VariableMetadata<int> metadata{};

    const auto min_value = Cool::String::find_value_for_given_key<int>(key_values, "min");
    if (min_value)
    {
        metadata.min_value = *min_value;
    }

    const auto max_value = Cool::String::find_value_for_given_key<int>(key_values, "max");
    if (max_value)
    {
        metadata.max_value = *max_value;
    }
    return metadata;
}

template<>
auto get_default_metadata(std::string_view key_values) -> Cool::VariableMetadata<float>
{
    Cool::VariableMetadata<float> metadata{};

    const auto min_value = Cool::String::find_value_for_given_key<float>(key_values, "min");
    if (min_value)
    {
        metadata.min_value = *min_value;
    }

    const auto max_value = Cool::String::find_value_for_given_key<float>(key_values, "max");
    if (max_value)
    {
        metadata.max_value = *max_value;
    }
    return metadata;
}

template<>
auto get_default_metadata(std::string_view) -> Cool::VariableMetadata<glm::vec2>
{
    Cool::VariableMetadata<glm::vec2> metadata{};
    return metadata;
}

template<>
auto get_default_metadata(std::string_view) -> Cool::VariableMetadata<glm::vec3>
{
    Cool::VariableMetadata<glm::vec3> metadata{};
    return metadata;
}

template<>
auto get_default_metadata(std::string_view) -> Cool::VariableMetadata<glm::vec4>
{
    Cool::VariableMetadata<glm::vec4> metadata{};
    return metadata;
}

template<>
auto get_default_metadata(std::string_view) -> Cool::VariableMetadata<Cool::Angle>
{
    Cool::VariableMetadata<Cool::Angle> metadata{};
    return metadata;
}

template<>
auto get_default_metadata(std::string_view) -> Cool::VariableMetadata<Cool::Direction2D>
{
    Cool::VariableMetadata<Cool::Direction2D> metadata{};
    return metadata;
}

template<>
auto get_default_metadata(std::string_view) -> Cool::VariableMetadata<Cool::Hue>
{
    Cool::VariableMetadata<Cool::Hue> metadata{};
    return metadata;
}
