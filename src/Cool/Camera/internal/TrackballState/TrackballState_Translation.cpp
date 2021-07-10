#include "TrackballState_Translation.h"
#include "TrackballState_Idle.h"

#include "../../ViewController_Trackball.h"
#include "../../Camera.h"

namespace Cool {

void TrackballState_Translation::on_mouse_move(ViewController_Trackball& controller, Camera& camera, glm::vec2 const& delta) {
	camera.translate(controller._translation_speed * (
		- delta.x * camera.right_axis() +
		+ delta.y * camera.up_axis()
	));
}

void TrackballState_Translation::on_wheel_up(ViewController_Trackball& controller, Camera& camera) {
	controller.set_state(TrackballState_Idle{});
}

} // namespace Cool