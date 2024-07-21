/* -----------------------------------------------------------------------------
 * This file was automatically generated by a Python script.
 * PLEASE DON'T EDIT IT DIRECTLY, your changes would be overwritten the next time the script is run.
 * Instead, go to "generator_variables.py" and edit the "Variable_Direction2D" function there.
 * -----------------------------------------------------------------------------
 */

#include <Cool/StrongTypes/Direction2D.h>
#include <Cool/Variables/Variable.h>
#include <Cool/Variables/internal/BoundsMetadata.h>

namespace Cool {

template<>
struct VariableMetadata<Cool::Direction2D> {
    int   number_of_snaps{24};
    float snaps_offset{0.f};
    bool  always_snap{false};

    friend auto operator<=>(VariableMetadata<Cool::Direction2D> const&, VariableMetadata<Cool::Direction2D> const&) = default;

private:
    // Serialisation
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            ser20::make_nvp("Number of snaps", number_of_snaps),
            ser20::make_nvp("Snaps offset (in radians)", snaps_offset),
            ser20::make_nvp("Always snap", always_snap)
        );
    }
};

auto imgui_widget(Variable<Cool::Direction2D>&) -> bool;
auto imgui_widget(VariableMetadata<Cool::Direction2D>&) -> bool;

} // namespace Cool
