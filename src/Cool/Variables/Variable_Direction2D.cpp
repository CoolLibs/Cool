#include "Variable_Direction2D.h"
#include "internal/angle_utils.h"

namespace Cool {

auto imgui_widget(Variable<Direction2D>& var) -> bool
{
    return imgui_widget(var.name(), var.value(), var.metadata().number_of_snaps, var.metadata().snaps_offset, var.metadata().always_snap);
}

auto imgui_widget(VariableMetadata<Direction2D>& meta) -> bool
{
    return internal::angle_metadata_widget(meta.number_of_snaps, meta.snaps_offset, meta.always_snap);
}

} // namespace Cool