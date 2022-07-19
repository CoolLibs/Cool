#include <Cool/Dependencies/Dirty.h>
#include <Cool/Dependencies/InputFactory_Ref.h>
#include <Cool/StrongTypes/RgbColor.h>
#include <Cool/Variables/Variable.h>
#include <sstream>

namespace Cool {

template<typename T>
auto get_default_value(std::string_view key_values) -> T
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
auto get_default_metadata(std::string_view) -> Cool::VariableMetadata<T>
{
    static_assert("No implementation found for this type! You can add it in generate_variables.py");
}

template<>
auto get_default_metadata(std::string_view key_values) -> Cool::VariableMetadata<Cool::RgbColor>;

template<typename T>
auto make_input(
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

auto make_any_input(
    const std::string_view type,
    const std::string_view name,
    DirtyFlag              dirty_flag,
    InputFactory_Ref       input_factory,
    std::string_view       key_values
) -> AnyInput;

auto is_input_declaration(std::string line) -> bool;

auto is_commented_out(std::string line) -> bool;

auto try_parse_input(
    std::string      line,
    DirtyFlag        dirty_flag,
    InputFactory_Ref input_factory
) -> std::optional<AnyInput>;

auto get_inputs_from_shader_code(
    std::string_view source_code,
    DirtyFlag        dirty_flag,
    InputFactory_Ref input_factory
) -> std::vector<AnyInput>;

} // namespace Cool