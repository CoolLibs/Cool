#pragma once
#if defined(COOL_OPENGL)

#include <Cool/Gpu/OpenGL/Shader.h>
#include "ParameterDynamicList.h"

namespace Cool {

template<typename Param>
void set_uniform(const OpenGL::Shader& shader, const Param& param)
{
    shader.set_uniform(param.name(), *param);
}

void set_uniform(const OpenGL::Shader& shader, const ParameterDynamicList& list)
{
    for (const auto& param : *list) {
        std::visit([&](auto&& param) { set_uniform(shader, param); }, param);
    }
}

} // namespace Cool

#endif