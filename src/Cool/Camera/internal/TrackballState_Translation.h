#pragma once

#include "TrackballState.h"

namespace Cool {

class TrackballState_Translation : public TrackballState {
public:
	TrackballState_Translation(ViewController_Trackball& controller);

	void update() override;
	void on_wheel_up() override;

private:
	glm::vec3 _initial_look_at;
	// _sc means that it is in screen coordinates
	glm::vec2 _initial_mouse_pos_sc;
};

} // namespace Cool