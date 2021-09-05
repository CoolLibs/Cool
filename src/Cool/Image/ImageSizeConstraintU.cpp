#include "ImageSizeConstraintU.h"

namespace Cool::ImageSizeConstraintU {

void apply(const ImageSizeConstraint& constraint, RenderTargetWindow& window)
{
    if (window.size().has_value()) {
        window->set_constrained_size(constraint.constrained_size(*window.size()));
        window.set_is_aspect_ratio_constrained(constraint.wants_to_constrain_aspect_ratio());
    }
}

} // namespace Cool::ImageSizeConstraintU