#include "TrackballState_Translation.h"
#include "TrackballState_Idle.h"

#include "../ViewController_Trackball.h"
#include "../Camera.h"
#include <Cool/App/Input.h>

namespace Cool {

TrackballState_Translation::TrackballState_Translation(ViewController_Trackball& controller)
	: TrackballState(controller),
	  _initial_look_at(controller._look_at),
	  _initial_mouse_pos_sc(Input::MouseInScreenCoordinates())
{}

void TrackballState_Translation::update() {
	glm::vec2 delta = (_initial_mouse_pos_sc - Input::MouseInScreenCoordinates()) * 0.01f;
	_controller._look_at =
		_initial_look_at
		+ _controller._camera.right_axis() * delta.x
		- _controller._camera.up_axis()    * delta.y;
	_controller.update_transform_matrix();
}

void TrackballState_Translation::on_wheel_up() {
	_controller.set_state<TrackballState_Idle>();
}

} // namespace Cool