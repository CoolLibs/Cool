#pragma once

#include "internal/Parameter_Color.h"
#include "internal/Parameter_Dir3.h"
#include "internal/Parameter_Float.h"
#include "internal/Parameter_Int.h"
#include "internal/Parameter_Vec2.h"

namespace Cool {

using AnyParameter = std::variant<
    Parameter::Int,
    Parameter::Float,
    Parameter::Vec2,
    Parameter::Dir3,
    Parameter::Color>;

} // namespace Cool