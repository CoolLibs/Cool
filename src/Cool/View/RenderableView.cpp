#include "RenderableView.h"

namespace Cool {

void RenderableView::update_size(ImageSizeConstraint const& constraint)
{
    if (!window_size().has_value())
        return;
    _render_target.set_size(constraint.applied_to(*window_size()));
}

auto RenderableView::get_image_texture_id() const -> ImTextureID
{
    return _render_target.imgui_texture_id();
}
auto RenderableView::get_image_size_inside_view() const -> ImageSizeInsideView
{
    return _render_target.current_size();
}

} // namespace Cool