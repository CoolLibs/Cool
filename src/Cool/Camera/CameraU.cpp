#include "CameraU.h"
#include <Cool/Constants/Constants.h>
#include <glm/gtx/vector_angle.hpp>
#include <smart/smart.hpp>

namespace Cool::CameraU {

float roll(const Camera& camera)
{
    const auto right         = camera.right_axis();
    const auto desired_right = glm::normalize(glm::cross(camera.front_axis(), Constants::world_up));
    return glm::angle(right, desired_right) * smart::sign(glm::dot(right, Constants::world_up));
}

void reset_roll(Camera& camera)
{
    camera.rotate_around(camera.position(),
                         roll(camera),
                         camera.front_axis());
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
