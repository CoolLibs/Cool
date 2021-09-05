#pragma once

#include <Cool/Gpu/RenderTargetWindow.h>
#include "ImageSizeConstraint.h"

namespace Cool::ImageSizeConstraintU {

void apply(const ImageSizeConstraint& constraint, RenderTargetWindow& window);

} // namespace Cool::ImageSizeConstraintU