#pragma once

#include <Cool/Gpu/OpenGL/Shader.h>

namespace Cool::CameraShaderU {

/**
 * @brief Sets the camera uniform in a shader that is including Cool's camera.glsl
 */
inline void set_uniform(const OpenGL::Shader& shader, const Camera& camera, float aspect_ratio)
{
    shader.set_uniform("cool_camera_inverse_view_projection", camera.inverse_view_projection_matrix(aspect_ratio));
    shader.set_uniform("cool_camera_far_plane", camera.projection().far_plane);
}

} // namespace Cool::CameraShaderU
