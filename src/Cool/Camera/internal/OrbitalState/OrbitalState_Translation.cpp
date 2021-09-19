#include "OrbitalState_Translation.h"
#include "../../Camera.h"
#include "../../ViewController_Orbital.h"
#include "OrbitalState_Idle.h"

namespace Cool {

void OrbitalState_Translation::on_drag(ViewController_Orbital& controller, Camera& camera, glm::vec2 const& delta)
{
    camera.translate(controller._translation_speed * (-delta.x * camera.right_axis() +
                                                      -delta.y * camera.up_axis()));
}

void OrbitalState_Translation::on_drag_stop(ViewController_Orbital& controller, Camera& camera)
{
    controller.set_state(OrbitalState_Idle{});
}

} // namespace Cool