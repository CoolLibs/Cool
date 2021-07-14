#pragma once

#include "IOrbitalState.h"

namespace Cool {

class OrbitalState_Idle : public IOrbitalState {
public:
    void on_wheel_down(ViewController_Orbital& controller, Camera& camera, int mods) override;
    void on_wheel_scroll(ViewController_Orbital& controller, Camera& camera, float dl) override;
};

} // namespace Cool