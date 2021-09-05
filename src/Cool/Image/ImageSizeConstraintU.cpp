#include "ImageSizeConstraintU.h"

namespace Cool::ImageSizeConstraintU {

void apply(const ImageSizeConstraint& constraint, RenderTargetWindow& window)
{
    if (window.size().has_value()) {
        window.set_preview_size(constraint.constrained_size(*window.size()));
    }
}

} // namespace Cool::ImageSizeConstraintU