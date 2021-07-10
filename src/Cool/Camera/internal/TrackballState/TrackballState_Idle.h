#pragma once

#include "ITrackballState.h"

namespace Cool {

class TrackballState_Idle : public ITrackballState {
public:
	void on_wheel_down  (ViewController_Trackball& controller, Camera& camera, int mods) override;
	void on_wheel_scroll(ViewController_Trackball& controller, Camera& camera, float dl) override;
};

} // namespace Cool