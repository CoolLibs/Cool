#pragma once

#include "IOrbitalState.h"

namespace Cool {

class OrbitalState_Translation : public IOrbitalState {
public:
    void on_drag(ViewController_Orbital& controller, Camera& camera, glm::vec2 const& delta) override;
    void on_drag_stop(ViewController_Orbital& controller, Camera&) override;
};

} // namespace Cool