/* -----------------------------------------------------------------------------
 * This file was automatically generated by a Python script.
 * PLEASE DON'T EDIT IT DIRECTLY, your changes would be overwritten the next time the script is run.
 * Instead, go to "generator_variables.py" and edit the "Variable_Hue" function there.
 * -----------------------------------------------------------------------------
 */

#include <Cool/StrongTypes/Hue.h>
#include <Cool/Variables/Variable.h>

namespace Cool {

template<>
struct VariableMetadata<Cool::Hue> {
    friend auto operator<=>(const VariableMetadata<Cool::Hue>&, const VariableMetadata<Cool::Hue>&) = default;

private:
    // Serialisation
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive&)
    {
    }
};

auto imgui_widget(Variable<Cool::Hue>&) -> bool;
auto imgui_widget(VariableMetadata<Cool::Hue>&) -> bool;

} // namespace Cool
