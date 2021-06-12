#pragma once

#include "internal/Param_Int.h"
#include "internal/Param_Float.h"
#include "internal/Param_Vec2.h"
#include "internal/Param_Dir3.h"
#include "internal/Param_Color.h"

namespace Cool {

using AnyParameter = std::variant<
      Param::Int,
      Param::Float,
      Param::Vec2,
      Param::Dir3,
      Param::Color
>;

} // namespace Cool