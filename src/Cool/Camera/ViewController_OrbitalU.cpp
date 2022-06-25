#include "ViewController_OrbitalU.h"
#include <Cool/Constants/Constants.h>
#include "CameraU.h"

namespace Cool::ViewController_OrbitalU {

void reset_transform(ViewController_Orbital& controller, Camera& camera)
{
    const auto position = Constants::default_camera_position;
    const auto look_at  = glm::vec3{0.f};

    camera = Camera{position, look_at};
    controller.set_distance_to_orbit_center(glm::distance(position, look_at));
}

void set_distance_to_orbit_center(ViewController_Orbital& controller, Camera& camera, float distance)
{
    CameraU::set_translation(
        camera,
        controller.get_orbit_center(camera) - distance * camera.front_axis()
    );
    controller.set_distance_to_orbit_center(distance);
}

} // namespace Cool::ViewController_OrbitalU
