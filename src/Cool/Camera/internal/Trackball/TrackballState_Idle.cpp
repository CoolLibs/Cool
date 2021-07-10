#include "TrackballState_Idle.h"
#include "TrackballState_Rotation.h"
#include "TrackballState_Translation.h"

#include "../../ViewController_Trackball.h"
#include "../../Camera.h"
#include <Cool/App/Input.h>

namespace Cool {

void TrackballState_Idle::on_wheel_down(ViewController_Trackball& controller, int mods) {
	if (mods & GLFW_MOD_SHIFT){
		controller.set_state(TrackballState_Translation{controller});
	}
	else {
		controller.set_state(TrackballState_Rotation{controller});
	}
}

void TrackballState_Idle::on_wheel_scroll(ViewController_Trackball& controller, float dl){
	controller._dist_to_look_at *= pow(0.93f, dl);
	controller.update_transform_matrix();
}

} // namespace Cool