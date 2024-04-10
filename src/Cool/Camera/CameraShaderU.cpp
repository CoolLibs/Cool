#include "CameraShaderU.h"

namespace Cool::CameraShaderU {

void set_uniform(FullscreenPipelineGLSL const& pipeline, Camera const& camera, float aspect_ratio)
{
    pipeline.set_uniform_with_name("cool_camera_inverse_view_projection", camera.inverse_view_projection_matrix(aspect_ratio));
    pipeline.set_uniform_with_name("cool_camera_view_projection", camera.view_projection_matrix(aspect_ratio));
    pipeline.set_uniform_with_name("cool_camera_far_plane", camera.projection().far_plane);
}

} // namespace Cool::CameraShaderU