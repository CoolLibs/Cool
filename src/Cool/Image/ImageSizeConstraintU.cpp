#include "ImageSizeConstraintU.h"

namespace Cool::ImageSizeConstraintU {

void apply(const ImageSizeConstraint& constraint, View& view)
{
    if (view.size().has_value()) {
        view.set_preview_size(constraint.constrained_size(*view.size()));
    }
}

} // namespace Cool::ImageSizeConstraintU