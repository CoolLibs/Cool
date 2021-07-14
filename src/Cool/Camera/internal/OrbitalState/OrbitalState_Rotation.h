#pragma once

#include "IOrbitalState.h"

namespace Cool {

class OrbitalState_Rotation : public IOrbitalState {
public:
    void on_mouse_move(ViewController_Orbital& controller, Camera& camera, glm::vec2 const& delta) override;
    void on_wheel_up(ViewController_Orbital& controller, Camera& camera) override;
};

} // namespace Cool