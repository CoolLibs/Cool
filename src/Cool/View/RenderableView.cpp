#include "RenderableView.h"

namespace Cool {

void RenderableView::update_size(ImageSizeConstraint constraint)
{
    if (view.size())
    {
        render_target.set_size(constraint.applied_to(*view.size()));
    }
}

void RenderableView::imgui_window(ViewWindowParams const& params)
{
    view.imgui_window(
        render_target.imgui_texture_id(),
        ImageSizeInsideView{render_target.current_size()},
        params
    );
}

} // namespace Cool