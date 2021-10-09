#pragma once
#if defined(COOL_OPENGL)

#include <Cool/Log/ToUser.h>
#include "Shader.h"

namespace Cool::OpenGL {

inline std::optional<Shader> try_make_shader(const std::vector<ShaderDescription>& shader_descriptions)
{
    try {
        return Shader{shader_descriptions};
    }
    catch (const std::invalid_argument& e) {
        Log::ToUser::error("Shader", "{}", e.what());
        return std::nullopt;
    }
}

} // namespace Cool::OpenGL

#endif