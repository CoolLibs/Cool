#pragma once
#include "SharedVariableDefinition.h"

namespace Cool {

auto always_requires_shader_code_generation(AnySharedVariableDefinition const&) -> bool;

}