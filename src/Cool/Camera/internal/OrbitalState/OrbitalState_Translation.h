#pragma once

#include "IOrbitalState.h"

namespace Cool {

class OrbitalState_Translation : public IOrbitalState {
public:
    auto on_drag(ViewController_Orbital& controller, Camera& camera, ImGuiCoordinates const& delta) -> bool override;
    auto on_drag_stop(ViewController_Orbital& controller, Camera&) -> bool override;
};

} // namespace Cool