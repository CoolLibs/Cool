#include "CameraU.h"
#include <Cool/Constants/Constants.h>

namespace Cool::CameraU {

float roll(const Camera& camera)
{
    return glm::dot(camera.right_axis(), Constants::world_up);
}

Ray ray_passing_through_pixel(const Camera& camera, glm::vec2 position_in_pixels, ImageSizeT<float> image_size)
{
    const auto pos       = camera.position();
    const auto pixel_pos = glm::unProject(glm::vec3{position_in_pixels, 0.f},
                                          camera.view_matrix(),
                                          camera.projection_matrix(),
                                          glm::vec4{0.f, 0.f, image_size.width(), image_size.height()});
    const auto dir       = pixel_pos - pos;
    return Ray{pos, dir};
}

} // namespace Cool::CameraU
