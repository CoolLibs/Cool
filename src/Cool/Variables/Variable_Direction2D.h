#pragma once

#include <Cool/StrongTypes/Direction2D.h>
#include "Variable.h"

namespace Cool {

#include "generated/Variable_Direction2D.inl"

auto imgui_widget(Variable<Direction2D>&) -> bool;
auto imgui_widget(VariableMetadata<Direction2D>&) -> bool;

} // namespace Cool
