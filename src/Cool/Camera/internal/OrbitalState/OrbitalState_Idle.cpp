#include "OrbitalState_Idle.h"
#include "../../Camera.h"
#include "../../ViewController_Orbital.h"
#include "OrbitalState_Rotation.h"
#include "OrbitalState_Translation.h"

namespace Cool {

void OrbitalState_Idle::on_drag_start(ViewController_Orbital& controller, Camera& camera, ModifierKeys mods)
{
    if (mods.shift()) {
        controller.set_state(OrbitalState_Translation{});
    }
    else {
        controller.set_state(OrbitalState_Rotation{camera});
    }
}

void OrbitalState_Idle::on_wheel_scroll(ViewController_Orbital& controller, Camera& camera, float dl)
{
    // Update controller's distance to look at
    const float previous_distance = controller._distance_to_orbit_center;
    controller._distance_to_orbit_center *= std::pow(0.93f, dl);
    // Update camera
    const float delta = previous_distance - controller._distance_to_orbit_center;
    camera.translate(camera.front_axis() * delta);
}

} // namespace Cool