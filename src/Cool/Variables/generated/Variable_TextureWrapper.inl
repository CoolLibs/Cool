/* -----------------------------------------------------------------------------
 * This file was automatically generated by a Python script.
 * PLEASE DON'T EDIT IT DIRECTLY, your changes would be overwritten the next time the script is run.
 * Instead, go to "generator_variables.py" and edit the "Variable_TextureWrapper" function there.
 * -----------------------------------------------------------------------------
 */

#include <Cool/StrongTypes/TextureWrapper.h>
#include <Cool/Variables/Variable.h>
#include <Cool/Variables/internal/BoundsMetadata.h>

namespace Cool {

template<>
struct VariableMetadata<Cool::TextureWrapper> {
    friend auto operator<=>(const VariableMetadata<Cool::TextureWrapper>&, const VariableMetadata<Cool::TextureWrapper>&) = default;

private:
    // Serialisation
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive&)
    {
    }
};

auto imgui_widget(Variable<Cool::TextureWrapper>&) -> bool;
auto imgui_widget(VariableMetadata<Cool::TextureWrapper>&) -> bool;

} // namespace Cool
