#include "Variable_ColorPalette.h"
#include "internal/color_utils.h"

namespace Cool {

auto imgui_widget(Variable<ColorPalette>& var) -> bool
{
    return imgui_widget(
        var.name,
        var.value,
        internal::color_imgui_flags(var.metadata.is_hdr)
    );
}

auto imgui_widget(VariableMetadata<ColorPalette>& meta) -> bool
{
    return internal::color_metadata_widget(meta.is_hdr);
}

} // namespace Cool