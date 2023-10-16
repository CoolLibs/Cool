#include "Variable_Camera2D.h"
#include <Cool/ImGui/ImGuiExtras.h>
#include "internal/angle_utils.h"

namespace Cool {

auto imgui_widget(Variable<Camera2D>& var) -> bool
{
    return imgui_widget(
        var.name,
        var.value,
        var.metadata.number_of_snaps,
        var.metadata.snaps_offset,
        var.metadata.always_snap
    );
}

auto imgui_widget(Metadata<Camera2D>& meta) -> bool
{
    return internal::angle_metadata_widget(meta.number_of_snaps, meta.snaps_offset, meta.always_snap);
}

} // namespace Cool