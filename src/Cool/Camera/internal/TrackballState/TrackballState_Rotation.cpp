#include "TrackballState_Rotation.h"
#include "TrackballState_Idle.h"

#include "../../ViewController_Trackball.h"
#include "../../Camera.h"
#include <Cool/App/Input.h>

namespace Cool {

TrackballState_Rotation::TrackballState_Rotation(const ViewController_Trackball& controller)
	: _initial_angle_ground(controller._angle_ground)
	, _initial_angle_up    (controller._angle_up)
	, _initial_mouse_pos_sc(Input::MouseInScreenCoordinates())
{}

void TrackballState_Rotation::update(ViewController_Trackball& controller) {
	glm::vec2 delta = (Input::MouseInScreenCoordinates() - _initial_mouse_pos_sc) * 0.01f;
	controller._angle_ground = _initial_angle_ground + delta.x;
	controller._angle_up     = _initial_angle_up     + delta.y;
	controller.update_transform_matrix();
}

void TrackballState_Rotation::on_wheel_up(ViewController_Trackball& controller) {
	controller.set_state(TrackballState_Idle{});
}

} // namespace Cool