#include "ViewController_OrbitalU.h"

namespace Cool::ViewController_OrbitalU {

void reset_transform(ViewController_Orbital& controller, Camera& camera)
{
    const auto position = glm::vec3{15.f, 0.f, 0.f};
    const auto look_at  = glm::vec3{0.f};

    camera = Camera{position, look_at};
    controller.set_distance_to_orbit_center(glm::distance(position, look_at));
}

} // namespace Cool::ViewController_OrbitalU
