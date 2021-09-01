#pragma once

#include <Cool/Gpu/RenderTarget.h>
#include "ImageSizeConstraint.h"

namespace Cool::ImageSizeConstraintU {

void apply(const ImageSizeConstraint& constraint, RenderTarget& render_target);

} // namespace Cool::ImageSizeConstraintU