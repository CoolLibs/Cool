#pragma once

#include "ShaderKind.h"

namespace Cool {

struct ShaderDescription {
    ShaderKind  kind;
    std::string source_code;
};

} // namespace Cool
