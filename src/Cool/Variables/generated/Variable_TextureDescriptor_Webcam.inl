/* -----------------------------------------------------------------------------
 * This file was automatically generated by a Python script.
 * PLEASE DON'T EDIT IT DIRECTLY, your changes would be overwritten the next time the script is run.
 * Instead, go to "generator_variables.py" and edit the "Variable_TextureDescriptor_Webcam" function there.
 * -----------------------------------------------------------------------------
 */

#include <Cool/TextureSource/TextureDescriptor.h>
#include <Cool/Variables/Variable.h>
#include <Cool/Variables/internal/BoundsMetadata.h>

namespace Cool {

template<>
struct VariableMetadata<Cool::TextureDescriptor_Webcam> {
    friend auto operator<=>(VariableMetadata<Cool::TextureDescriptor_Webcam> const&, VariableMetadata<Cool::TextureDescriptor_Webcam> const&) = default;

private:
    // Serialisation
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive&)
    {
    }
};

auto imgui_widget(Variable<Cool::TextureDescriptor_Webcam>&) -> bool;
auto imgui_widget(VariableMetadata<Cool::TextureDescriptor_Webcam>&) -> bool;

} // namespace Cool
