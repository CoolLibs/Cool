#pragma once

#include "TrackballState.h"

namespace Cool {

class TrackballState_Rotation : public TrackballState {
public:
	TrackballState_Rotation(ViewController_Trackball& controller);

	void update() override;
	void on_wheel_up() override;

private:
	float _initial_angle_ground;
	float _initial_angle_up;
	// _sc means that it is in screen coordinates
	glm::vec2 _initial_mouse_pos_sc;
};

} // namespace Cool