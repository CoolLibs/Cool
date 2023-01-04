#include "Variable_Direction2D.h"
#include "internal/angle_utils.h"

namespace Cool {

auto imgui_widget(Variable<Direction2D>& var) -> bool
{
    return imgui_widget(var.name, var.value, var.metadata.number_of_snaps, var.metadata.snaps_offset);
}

auto imgui_widget(VariableMetadata<Direction2D>& meta) -> bool
{
    return internal::angle_metadata_widget(meta.number_of_snaps, meta.snaps_offset);
}

} // namespace Cool