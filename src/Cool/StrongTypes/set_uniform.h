#pragma once

#include <Cool/Dependencies/Input.h>
#include <Cool/Gpu/OpenGL/Shader.h>
#include "Cool/StrongTypes/MathExpression.h"

namespace Cool {

template<typename T>
static void set_uniform(Cool::OpenGL::Shader const& shader, std::string_view name, T const& value)
{
    shader.set_uniform(name, value);
}

static void set_uniform(Cool::OpenGL::Shader const& shader, std::string_view name, Cool::MathExpression const&)
{
    // We don't want any set_uniform() for MathExpression since we want to generate the shader code.
    return;
}

template<>
void set_uniform(Cool::OpenGL::Shader const&, std::string_view, Cool::Camera const&)
{
    assert(false); // This isn't used at the moment because we set the camera3d manually for all shaders, but this should be changed
    // Cool::CameraShaderU::set_uniform(shader, value);
}

} // namespace Cool