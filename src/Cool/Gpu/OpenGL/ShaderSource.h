#pragma once
#if defined(__COOL_APP_OPENGL)

#include "../ShaderKind.h"

namespace Cool::OpenGL {

class ShaderSource {
public:
    ShaderSource(std::string_view glsl_source, ShaderKind shader_kind)
        : _glsl_source{glsl_source}, _shader_kind{shader_kind}
    {
    }

    const std::string& glsl_source() const { return _glsl_source; }
    ShaderKind         kind() const { return _shader_kind; }

private:
    std::string _glsl_source;
    ShaderKind  _shader_kind;
};

} // namespace Cool::OpenGL

#endif