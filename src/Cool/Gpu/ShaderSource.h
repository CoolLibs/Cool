#pragma once

#include <Cool/File/File.h>
#include "ShaderKind.h"

namespace Cool {

class ShaderSource {
public:
    explicit ShaderSource(std::string_view glsl_source)
        : _glsl_source{glsl_source}
    {
    }

    std::string_view glsl_source() const { return _glsl_source; }

private:
    std::string _glsl_source;
};

} // namespace Cool