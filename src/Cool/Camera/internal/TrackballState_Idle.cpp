#include "TrackballState_Idle.h"
#include "TrackballState_Rotation.h"
#include "TrackballState_Translation.h"

#include "../ViewController_Trackball.h"
#include "../Camera.h"
#include <Cool/App/Input.h>

namespace Cool {

TrackballState_Idle::TrackballState_Idle(ViewController_Trackball& controller)
	: TrackballState(controller)
{}

void TrackballState_Idle::on_wheel_down(int mods) {
	if (mods & GLFW_MOD_SHIFT){
		_controller.set_state<TrackballState_Translation>();
	}
	else {
		_controller.set_state<TrackballState_Rotation>();
	}
}

void TrackballState_Idle::on_wheel_scroll(float dl){
	_controller._dist_to_look_at *= pow(0.93f, dl);
	_controller.update_transform_matrix();
}

} // namespace Cool