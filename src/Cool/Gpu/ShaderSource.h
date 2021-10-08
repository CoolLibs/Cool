#pragma once

#include <Cool/File/File.h>
#include "ShaderKind.h"

namespace Cool {

class ShaderSource {
public:
    explicit ShaderSource(std::string_view glsl_source);

    const std::string& glsl_source() const { return _glsl_source; }

private:
    std::string _glsl_source;
};

} // namespace Cool