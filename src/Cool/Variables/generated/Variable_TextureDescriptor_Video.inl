/* -----------------------------------------------------------------------------
 * This file was automatically generated by a Python script.
 * PLEASE DON'T EDIT IT DIRECTLY, your changes would be overwritten the next time the script is run.
 * Instead, go to "generator_variables.py" and edit the "Variable_TextureDescriptor_Video" function there.
 * -----------------------------------------------------------------------------
 */

#include <Cool/TextureSource/TextureDescriptor.h>
#include <Cool/Variables/Variable.h>
#include <Cool/Variables/internal/BoundsMetadata.h>

namespace Cool {

template<>
struct VariableMetadata<Cool::TextureDescriptor_Video> {
    friend auto operator<=>(VariableMetadata<Cool::TextureDescriptor_Video> const&, VariableMetadata<Cool::TextureDescriptor_Video> const&) = default;

private:
    // Serialisation
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive&)
    {
    }
};

auto imgui_widget(Variable<Cool::TextureDescriptor_Video>&) -> bool;
auto imgui_widget(VariableMetadata<Cool::TextureDescriptor_Video>&) -> bool;

} // namespace Cool
