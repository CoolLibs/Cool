#pragma once

#include <Cool/StrongTypes/Angle.h>
#include "Variable.h"

namespace Cool {

#include "generated/Variable_Angle.inl"

auto imgui_widget(Variable<Angle>&) -> bool;
auto imgui_widget(VariableMetadata<Angle>&) -> bool;

} // namespace Cool
