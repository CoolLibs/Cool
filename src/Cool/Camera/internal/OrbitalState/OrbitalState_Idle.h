#pragma once

#include "IOrbitalState.h"

namespace Cool {

class OrbitalState_Idle : public IOrbitalState {
public:
    auto on_drag_start(ViewController_Orbital& controller, Camera& camera) -> bool override;
    auto on_wheel_scroll(ViewController_Orbital& controller, Camera& camera, float dl) -> bool override;
};

} // namespace Cool