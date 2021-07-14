#include "OrbitalState_Rotation.h"
#include <Cool/Constants/Constants.h>
#include <Cool/Utils/sign.h>
#include "../../Camera.h"
#include "../../ViewController_Orbital.h"
#include "OrbitalState_Idle.h"

namespace Cool {

void OrbitalState_Rotation::on_mouse_move(ViewController_Orbital& controller, Camera& camera, glm::vec2 const& delta)
{
    const auto orbit_center = controller.get_orbit_center(camera);
    const auto up_axis      = [&]() {
        switch (controller._mode) {
        case ViewController_Orbital::Mode::Trackball:
            return Constants::world_up;
        case ViewController_Orbital::Mode::Turntable: {
            const float sign = glm::dot(camera.up_axis(), Constants::world_up) > 0.f ? 1.f : -1.f;
            return sign * Constants::world_up;
        }
        case ViewController_Orbital::Mode::AxisFree:
            return camera.up_axis();
        default:
            Log::error("[OrbitalState_Rotation::on_mouse_move] Unknown ViewController_Orbital::Mode");
            return glm::vec3{0};
        }
    }();
    camera.translate(-orbit_center);
    camera.rotate(-controller._rotation_speed * delta.x, up_axis);
    camera.rotate(-controller._rotation_speed * delta.y, camera.right_axis());
    camera.translate(+orbit_center);
}

void OrbitalState_Rotation::on_wheel_up(ViewController_Orbital& controller, Camera& camera)
{
    controller.set_state(OrbitalState_Idle{});
}

} // namespace Cool