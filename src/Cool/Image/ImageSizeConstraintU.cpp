#include "ImageSizeConstraintU.h"

namespace Cool::ImageSizeConstraintU {

void apply(const ImageSizeConstraint& constraint, RenderTarget& render_target)
{
    render_target.set_constrained_size(constraint.constrained_size(render_target.imgui_window_size()));
    render_target.set_is_aspect_ratio_constrained(constraint.wants_to_constrain_aspect_ratio());
}

} // namespace Cool::ImageSizeConstraintU