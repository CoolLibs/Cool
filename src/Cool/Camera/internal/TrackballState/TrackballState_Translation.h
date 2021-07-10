#pragma once

#include "ITrackballState.h"

namespace Cool {

class TrackballState_Translation : public ITrackballState {
public:
	TrackballState_Translation(const ViewController_Trackball& controller);
	void update               (      ViewController_Trackball& controller) override;
	void on_wheel_up          (      ViewController_Trackball& controller) override;

private:
	glm::vec3 _initial_look_at;
	// _sc means that it is in screen coordinates
	glm::vec2 _initial_mouse_pos_sc;
};

} // namespace Cool