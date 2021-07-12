#include "OrbitalState_Idle.h"
#include "OrbitalState_Rotation.h"
#include "OrbitalState_Translation.h"

#include <GLFW/glfw3.h>
#include "../../ViewController_Orbital.h"
#include "../../Camera.h"

namespace Cool {

void OrbitalState_Idle::on_wheel_down(ViewController_Orbital& controller, Camera& camera, int mods) {
	if (mods & GLFW_MOD_SHIFT){
		controller.set_state(OrbitalState_Translation{});
	}
	else {
		controller.set_state(OrbitalState_Rotation{});
	}
}

void OrbitalState_Idle::on_wheel_scroll(ViewController_Orbital& controller, Camera& camera, float dl) {
	// Update controller's distance to look at
	const float previous_dist = controller._dist_to_look_at;
	controller._dist_to_look_at *= pow(0.93f, dl);
	// Update camera
	const float delta = previous_dist - controller._dist_to_look_at;
	camera.translate(camera.front_axis() * delta);
}

} // namespace Cool