#include "TrackballState_Idle.h"
#include "TrackballState_Rotation.h"
#include "TrackballState_Translation.h"

#include "../../ViewController_Trackball.h"
#include "../../Camera.h"
#include <Cool/App/Input.h>

namespace Cool {

void TrackballState_Idle::on_wheel_down(ViewController_Trackball& controller, Camera& camera, int mods) {
	if (mods & GLFW_MOD_SHIFT){
		controller.set_state(TrackballState_Translation{});
	}
	else {
		controller.set_state(TrackballState_Rotation{});
	}
}

void TrackballState_Idle::on_wheel_scroll(ViewController_Trackball& controller, Camera& camera, float dl) {
	// Update controller's distance to look at
	const float previous_dist = controller._dist_to_look_at;
	controller._dist_to_look_at *= pow(0.93f, dl);
	// Update camera
	const float delta = previous_dist - controller._dist_to_look_at;
	camera.translate(camera.front_axis() * delta);
}

} // namespace Cool