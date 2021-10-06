#pragma once

#include "IOrbitalState.h"

namespace Cool {

class OrbitalState_Rotation : public IOrbitalState {
public:
    explicit OrbitalState_Rotation(const Camera& camera);

    void on_drag(ViewController_Orbital& controller, Camera& camera, glm::vec2 const& delta) override;
    void on_drag_stop(ViewController_Orbital& controller, Camera&) override;

private:
    float _sign_of_alignment_with_up{};
};

} // namespace Cool