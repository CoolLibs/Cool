#pragma once

#include "ShaderKind.h"

namespace Cool {

struct ShaderDescription {
    std::string source_code;
    ShaderKind  kind;
};

} // namespace Cool
