#pragma once

#include <Cool/File/File.h>
#include "ShaderKind.h"


namespace Cool {

class ShaderSource {
public:
    ShaderSource(std::string_view glsl_source, ShaderKind shader_kind)
        : _glsl_source{glsl_source}, _shader_kind{shader_kind}
    {
    }

    ShaderSource from_file(std::string_view file_path, ShaderKind shader_kind)
    {
        return ShaderSource{File::to_string(file_path), shader_kind};
    }

    const std::string& glsl_source() const { return _glsl_source; }
    ShaderKind         kind() const { return _shader_kind; }

private:
    std::string _glsl_source;
    ShaderKind  _shader_kind;
};

} // namespace Cool