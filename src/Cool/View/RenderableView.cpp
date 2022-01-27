#include "RenderableView.h"

namespace Cool {

void RenderableView::update_size(ImageSizeConstraint constraint)
{
    if (view.size()) {
        render_target.set_size(constraint.applied_to(*view.size()));
    }
}

void RenderableView::imgui_window(bool aspect_ratio_is_constrained, std::function<bool()> extra)
{
    view.imgui_window(
        render_target.imgui_texture_id(),
        ImageSizeInsideView{render_target.current_size(),
                            aspect_ratio_is_constrained},
        extra);
}

} // namespace Cool