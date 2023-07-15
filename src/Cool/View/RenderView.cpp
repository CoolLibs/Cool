#include "RenderView.h"

namespace Cool {

void RenderView::update_size(ImageSizeConstraint const& constraint)
{
    if (!window_size().has_value())
        return;
    _render_target.set_size(constraint.applied_to(*window_size()));
}

auto RenderView::get_image_texture_id() const -> ImTextureID
{
    return _render_target.imgui_texture_id();
}
auto RenderView::get_image_size() const -> img::Size
{
    return _render_target.current_size();
}

} // namespace Cool