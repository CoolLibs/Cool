#pragma once
#include "Cool/Camera/Camera.h"
#include "Cool/WebGPU/FullscreenPipelineGLSL.h"

namespace Cool::CameraShaderU {

/// Sets the camera uniform in a shader that is including Cool's camera.glsl
void set_uniform(FullscreenPipelineGLSL const& pipeline, Camera const& camera, float aspect_ratio);

} // namespace Cool::CameraShaderU
