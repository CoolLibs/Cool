#pragma once

#include "IOrbitalState.h"

namespace Cool {

class OrbitalState_Rotation : public IOrbitalState {
public:
    explicit OrbitalState_Rotation(const Camera& camera);

    auto on_drag(ViewController_Orbital& controller, Camera& camera, glm::vec2 const& delta) -> bool override;
    auto on_drag_stop(ViewController_Orbital& controller, Camera&) -> bool override;

private:
    float _sign_of_alignment_with_up{};
};

} // namespace Cool