#include "Variable_Angle.h"
#include "Cool/Math/constants.h"
#include "internal/angle_utils.h"

namespace Cool {

auto imgui_widget(Variable<Angle>& var) -> bool
{
    return Cool::imgui_widget(var.name, var.value, var.metadata.number_of_snaps, var.metadata.snaps_offset * tau);
}

auto imgui_widget(VariableMetadata<Angle>& meta) -> bool
{
    return internal::angle_metadata_widget(meta.number_of_snaps, meta.snaps_offset);
}

} // namespace Cool