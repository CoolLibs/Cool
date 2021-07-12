#include "OrbitalState_Rotation.h"
#include "OrbitalState_Idle.h"

#include "../../ViewController_Orbital.h"
#include "../../Camera.h"
#include <Cool/Constants/Constants.h>

namespace Cool {

void OrbitalState_Rotation::on_mouse_move(ViewController_Orbital& controller, Camera& camera, glm::vec2 const& delta) {
	const auto look_at = controller.get_look_at(camera);
	camera.translate(-look_at);
	camera.rotate(- controller._rotation_speed * delta.x, Constants::world_up);
	camera.rotate(- controller._rotation_speed * delta.y, camera.right_axis());
	camera.translate(+look_at);
}

void OrbitalState_Rotation::on_wheel_up(ViewController_Orbital& controller, Camera& camera) {
	controller.set_state(OrbitalState_Idle{});
}

} // namespace Cool