#pragma once

#include "Camera.h"
#include "ViewController_Orbital.h"

namespace Cool::ViewController_OrbitalU {

void reset_transform(ViewController_Orbital& controller, Camera& camera);
void set_distance_to_orbit_center(ViewController_Orbital&, Camera&, float distance);

} // namespace Cool::ViewController_OrbitalU
