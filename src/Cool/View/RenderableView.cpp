#include "RenderableView.h"

namespace Cool {

void RenderableView::update_size(ImageSizeConstraint constraint)
{
    if (view.size()) {
        render_target.set_size(constraint.applied_to(*view.size()));
    }
}

void RenderableView::imgui_window(bool aspect_ratio_is_constrained)
{
    view.imgui_window(
        render_target.imgui_texture_id(),
        render_target.current_size(),
        aspect_ratio_is_constrained);
}

} // namespace Cool