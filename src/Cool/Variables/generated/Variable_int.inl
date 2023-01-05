/* -----------------------------------------------------------------------------
 * This file was automatically generated by a Python script.
 * PLEASE DON'T EDIT IT DIRECTLY, your changes would be overwritten the next time the script is run.
 * Instead, go to "generator_variables.py" and edit the "Variable_int" function there.
 * -----------------------------------------------------------------------------
 */

#include "Variable.h"

namespace Cool {

template<>
struct VariableMetadata<int> {
    int min_value{0};
    int max_value{10};

    friend auto operator<=>(const VariableMetadata<int>&, const VariableMetadata<int>&) = default;

private:
    // Serialisation
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Min Value", min_value),
            cereal::make_nvp("Max Value", max_value)
        );
    }
};

auto imgui_widget(Variable<int>&) -> bool;
auto imgui_widget(VariableMetadata<int>&) -> bool;

} // namespace Cool
