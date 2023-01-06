/* -----------------------------------------------------------------------------
 * This file was automatically generated by a Python script.
 * PLEASE DON'T EDIT IT DIRECTLY, your changes would be overwritten the next time the script is run.
 * Instead, go to "generator_variables.py" and edit the "Variable_Color" function there.
 * -----------------------------------------------------------------------------
 */

#include <Cool/StrongTypes/Color.h>
#include <Cool/Variables/Variable.h>

namespace Cool {

template<>
struct VariableMetadata<Cool::Color> {
    bool is_hdr{false};

    friend auto operator<=>(const VariableMetadata<Cool::Color>&, const VariableMetadata<Cool::Color>&) = default;

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

auto imgui_widget(Variable<Cool::Color>&) -> bool;
auto imgui_widget(VariableMetadata<Cool::Color>&) -> bool;

} // namespace Cool
