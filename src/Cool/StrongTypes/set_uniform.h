#pragma once

#include <Cool/Dependencies/Input.h>
#include <Cool/Gpu/OpenGL/Shader.h>

namespace Cool {

template<typename T>
static void set_uniform(Cool::OpenGL::Shader const& shader, std::string_view name, T const& value)
{
    shader.set_uniform(name, value);
}

template<>
void set_uniform(Cool::OpenGL::Shader const& shader, std::string_view name, Cool::RgbColor const& value)
{
    shader.set_uniform(name, value.value);
}

template<>
void set_uniform(Cool::OpenGL::Shader const&, std::string_view, Cool::Camera const&)
{
    assert(false); // This isn't used at the moment because we set the camera3d manually for all shaders, but this should be changed
    // Cool::CameraShaderU::set_uniform(shader, value);
}

} // namespace Cool