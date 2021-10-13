#pragma once
#if defined(COOL_OPENGL)

#include <Cool/Log/ToUser.h>
#include "Shader.h"

namespace Cool::OpenGL {

inline std::optional<Shader> try_make_shader(const std::vector<ShaderDescription>& shader_descriptions)
{
    try {
        std::vector<ShaderModule>        modules;
        std::vector<const ShaderModule*> modules_ref;
        modules.reserve(shader_descriptions.size());
        modules_ref.reserve(shader_descriptions.size());
        for (const auto& desc : shader_descriptions) {
            modules.emplace_back(desc);
            modules_ref.push_back(&modules.back());
        }
        return Shader{modules_ref};
    }
    catch (const std::invalid_argument& e) {
        Log::ToUser::error("Shader", "{}", e.what());
        return std::nullopt;
    }
}

} // namespace Cool::OpenGL

#endif