#pragma once

#include <Cool/Geometry/Ray.h>
#include <Cool/Image/ImageSize.h>
#include "Camera.h"

namespace Cool::CameraU {

float roll(const Camera& camera);
void  reset_roll(Camera& camera);

/**
 * @brief Returns a Ray passing through the given pixel of an image and starting at the camera position. Typically what you would want in order to check if a 3D object is under the mouse cursor by casting a ray : ray_passing_through_pixel(camera, mouse_position_in_pixels, image_size)
 */
Ray ray_passing_through_pixel(const Camera& camera, glm::vec2 position_in_pixels, ImageSizeT<float> image_size);

} // namespace Cool::CameraU
