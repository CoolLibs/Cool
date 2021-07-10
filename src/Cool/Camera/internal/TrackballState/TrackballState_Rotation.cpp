#include "TrackballState_Rotation.h"
#include "TrackballState_Idle.h"

#include "../../ViewController_Trackball.h"
#include "../../Camera.h"
#include <Cool/App/Input.h>

namespace Cool {

void TrackballState_Rotation::on_mouse_move(ViewController_Trackball& controller, Camera& camera, glm::vec2 const& delta) {
	const auto look_at = controller.get_look_at(camera);
	camera.translate(-look_at);
	camera.rotate(controller._rotation_speed * delta.x, {0.f, 1.f, 0.f});
	camera.rotate(controller._rotation_speed * delta.y, camera.right_axis());
	camera.translate(+look_at);
}

void TrackballState_Rotation::on_wheel_up(ViewController_Trackball& controller, Camera& camera) {
	controller.set_state(TrackballState_Idle{});
}

} // namespace Cool