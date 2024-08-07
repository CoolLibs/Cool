/* -----------------------------------------------------------------------------
 * This file was automatically generated by a Python script.
 * PLEASE DON'T EDIT IT DIRECTLY, your changes would be overwritten the next time the script is run.
 * Instead, go to "generator_variables.py" and edit the "Variable_Camera" function there.
 * -----------------------------------------------------------------------------
 */

#include <Cool/Camera/Camera.h>
#include <Cool/Variables/Variable.h>
#include <Cool/Variables/internal/BoundsMetadata.h>

namespace Cool {

template<>
struct VariableMetadata<Cool::Camera> {
    friend auto operator<=>(VariableMetadata<Cool::Camera> const&, VariableMetadata<Cool::Camera> const&) = default;

private:
    // Serialisation
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive&)
    {
    }
};

auto imgui_widget(Variable<Cool::Camera>&) -> bool;
auto imgui_widget(VariableMetadata<Cool::Camera>&) -> bool;

} // namespace Cool
