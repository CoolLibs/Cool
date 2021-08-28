#pragma once

#include "ShaderKind.h"

namespace Cool {

class ShaderSource {
public:
    ShaderSource(std::string_view vulkan_glsl_source, ShaderKind shader_kind);

    std::string to_opengl_glsl() const;
    ShaderKind  kind() const { return _shader_kind; }

private:
    std::string _vulkan_glsl_source;
    ShaderKind  _shader_kind;
};

} // namespace Cool