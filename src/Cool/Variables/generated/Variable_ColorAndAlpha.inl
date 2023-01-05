/* -----------------------------------------------------------------------------
 * This file was automatically generated by a Python script.
 * PLEASE DON'T EDIT IT DIRECTLY, your changes would be overwritten the next time the script is run.
 * Instead, go to "generator_variables.py" and edit the "Variable_ColorAndAlpha" function there.
 * -----------------------------------------------------------------------------
 */

#include <Cool/StrongTypes/ColorAndAlpha.h>
#include "Variable.h"

namespace Cool {

template<>
struct VariableMetadata<Cool::ColorAndAlpha> {
    bool is_hdr{false};

    friend auto operator<=>(const VariableMetadata<Cool::ColorAndAlpha>&, const VariableMetadata<Cool::ColorAndAlpha>&) = default;

private:
    // Serialisation
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Is HDR", is_hdr)
        );
    }
};

auto imgui_widget(Variable<Cool::ColorAndAlpha>&) -> bool;
auto imgui_widget(VariableMetadata<Cool::ColorAndAlpha>&) -> bool;

} // namespace Cool
