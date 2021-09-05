#pragma once

#include <Cool/Gpu/View.h>
#include "ImageSizeConstraint.h"

namespace Cool::ImageSizeConstraintU {

void apply(const ImageSizeConstraint& constraint, View& view);

} // namespace Cool::ImageSizeConstraintU