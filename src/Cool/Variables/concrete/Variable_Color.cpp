#include "Variable_Color.h"
#include "internal/color_utils.h"

namespace Cool {

auto imgui_widget(Variable<Color>& var) -> bool
{
    return imgui_widget(
        var.name,
        var.value,
        internal::color_imgui_flags(var.metadata.is_hdr)
    );
}

auto imgui_widget(Metadata<Color>& meta) -> bool
{
    return internal::color_metadata_widget(meta.is_hdr);
}

} // namespace Cool