#pragma once

#include "ITrackballState.h"

namespace Cool {

class TrackballState_Rotation : public ITrackballState {
public:
	TrackballState_Rotation(const ViewController_Trackball& controller);
	void update            (      ViewController_Trackball& controller) override;
	void on_wheel_up       (      ViewController_Trackball& controller) override;

private:
	float _initial_angle_ground;
	float _initial_angle_up;
	// _sc means that it is in screen coordinates
	glm::vec2 _initial_mouse_pos_sc;
};

} // namespace Cool