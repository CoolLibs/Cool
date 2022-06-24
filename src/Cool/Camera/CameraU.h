#pragma once

#include <Cool/Geometry/Ray.h>
#include <img/img.hpp>
#include "Camera.h"

namespace Cool::CameraU {

float roll(const Camera&);
void  reset_roll(Camera&);

void set_translation(Camera&, const glm::vec3& new_position);

/**
 * @brief Returns a Ray passing through the given pixel of an image and starting at the camera position. Typically what you would want in order to check if a 3D object is under the mouse cursor by casting a ray : ray_passing_through_pixel(camera, mouse_position_in_pixels, image_size)
 */
Ray ray_passing_through_pixel(const Camera&, glm::vec2 position_in_pixels, img::SizeT<float> image_size);

} // namespace Cool::CameraU
