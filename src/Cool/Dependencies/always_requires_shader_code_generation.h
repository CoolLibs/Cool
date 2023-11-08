#pragma once
#include "InputDefinition.h"

namespace Cool {

auto always_requires_shader_code_generation(AnyInputDefinition const&) -> bool;

}