#include "OrbitalState_Translation.h"
#include "../../Camera.h"
#include "../../ViewController_Orbital.h"
#include "OrbitalState_Idle.h"

namespace Cool {

auto OrbitalState_Translation::on_drag(ViewController_Orbital& controller, Camera& camera, glm::vec2 const& delta) -> bool
{
    camera.translate(
        controller._translation_speed * (-delta.x * camera.right_axis() +
                                         -delta.y * camera.up_axis())
    );
    return true;
}

auto OrbitalState_Translation::on_drag_stop(ViewController_Orbital& controller, Camera&) -> bool
{
    controller.set_state(OrbitalState_Idle{});
    return false;
}

} // namespace Cool