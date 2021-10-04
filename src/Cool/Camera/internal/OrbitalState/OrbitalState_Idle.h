#pragma once

#include "IOrbitalState.h"

namespace Cool {

class OrbitalState_Idle : public IOrbitalState {
public:
    void on_drag_start(ViewController_Orbital& controller, Camera&, ModifierKeys mods) override;
    void on_wheel_scroll(ViewController_Orbital& controller, Camera& camera, float dl) override;
};

} // namespace Cool