#pragma once
#include "Cool/Camera/Camera.h"
#include "Cool/StrongTypes/MathExpression.h"
#include "Cool/WebGPU/FullscreenPipelineGLSL.h"

namespace Cool {

template<typename T>
void set_uniform(Cool::FullscreenPipelineGLSL const& pipeline, std::string_view name, T const& value)
{
    pipeline.set_uniform_with_name(name, value);
}

template<>
inline void set_uniform(Cool::FullscreenPipelineGLSL const&, std::string_view, Cool::MathExpression const&)
{
    // We don't want any set_uniform() for MathExpression since we want to generate the shader code.
}

template<>
inline void set_uniform(Cool::FullscreenPipelineGLSL const&, std::string_view, Cool::Camera const&)
{
    assert(false); // This isn't used at the moment because we set the camera3d manually for all shaders, but this should be changed
    // Cool::CameraShaderU::set_uniform(pipeline, value);
}

} // namespace Cool