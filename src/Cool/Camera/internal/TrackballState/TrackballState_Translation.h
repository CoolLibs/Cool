#pragma once

#include "ITrackballState.h"

namespace Cool {

class TrackballState_Translation : public ITrackballState {
public:
	void on_mouse_move(ViewController_Trackball& controller, Camera& camera, glm::vec2 const& delta) override;
	void on_wheel_up  (ViewController_Trackball& controller, Camera& camera)                         override;
};

} // namespace Cool