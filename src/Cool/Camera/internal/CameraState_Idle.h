#pragma once

#include "CameraState.h"

namespace Cool {

class CameraState_Idle : public CameraState {
public:
	CameraState_Idle(Camera& camera);
	~CameraState_Idle() = default;

	void onWheelDown(int mods) override;
	void onWheelScroll(float dl) override;
};

} // namespace Cool