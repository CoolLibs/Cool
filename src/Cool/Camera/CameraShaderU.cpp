#include "CameraShaderU.h"

namespace Cool::CameraShaderU {

void set_uniform(Shader const& shader, Camera const& camera, float aspect_ratio)
{
    shader.set_uniform("cool_camera_inverse_view_projection", camera.inverse_view_projection_matrix(aspect_ratio));
    shader.set_uniform("cool_camera_view_projection", camera.view_projection_matrix(aspect_ratio));
    shader.set_uniform("cool_camera_far_plane", camera.projection().far_plane);
}

} // namespace Cool::CameraShaderU