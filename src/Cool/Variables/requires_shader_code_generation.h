#pragma once
#include "VariableDefinition.h"

namespace Cool {

auto requires_shader_code_generation(AnyVariableDefinition const&) -> bool;

}