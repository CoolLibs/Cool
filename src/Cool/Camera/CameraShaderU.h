#pragma once
#include <Cool/Gpu/OpenGL/Shader.h>
#include "Cool/Camera/Camera.h"

namespace Cool::CameraShaderU {

/// Sets the camera uniform in a shader that is including Cool's camera.glsl
void set_uniform(OpenGL::Shader const& shader, Camera const& camera, float aspect_ratio);

} // namespace Cool::CameraShaderU
