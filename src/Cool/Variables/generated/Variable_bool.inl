/* -----------------------------------------------------------------------------
 * This file was automatically generated by a Python script.
 * PLEASE DON'T EDIT IT DIRECTLY, your changes would be overwritten the next time the script is run.
 * Instead, go to "generator_variables.py" and edit the "Variable_bool" function there.
 * -----------------------------------------------------------------------------
 */

#include <Cool/Variables/Variable.h>
#include <Cool/Variables/internal/BoundsMetadata.h>

namespace Cool {

template<>
struct VariableMetadata<bool> {
    friend auto operator<=>(VariableMetadata<bool> const&, VariableMetadata<bool> const&) = default;

private:
    // Serialisation
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive&)
    {
    }
};

auto imgui_widget(Variable<bool>&) -> bool;
auto imgui_widget(VariableMetadata<bool>&) -> bool;

} // namespace Cool
