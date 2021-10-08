#pragma once

#include "ShaderKind.h"
#include "ShaderSource.h"

namespace Cool {

struct ShaderDescription {
    ShaderSource source_code;
    ShaderKind   kind;
};

} // namespace Cool
