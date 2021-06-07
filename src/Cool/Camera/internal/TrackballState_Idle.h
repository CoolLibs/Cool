#pragma once

#include "TrackballState.h"

namespace Cool {

class TrackballState_Idle : public TrackballState {
public:
	TrackballState_Idle(ViewController_Trackball& controller);

	void on_wheel_down(int mods) override;
	void on_wheel_scroll(float dl) override;
};

} // namespace Cool