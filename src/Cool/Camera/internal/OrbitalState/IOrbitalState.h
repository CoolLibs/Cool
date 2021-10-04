#pragma once
#include <Cool/Input/ModifierKeys.h>

namespace Cool {

class ViewController_Orbital;
class Camera;

class IOrbitalState {
public:
    virtual void on_drag(ViewController_Orbital& controller, Camera& camera, glm::vec2 const& delta){};
    virtual void on_wheel_scroll(ViewController_Orbital& controller, Camera& camera, float dl){};
    virtual void on_drag_start(ViewController_Orbital& controller, Camera& camera, ModifierKeys mods){};
    virtual void on_drag_stop(ViewController_Orbital& controller, Camera& camera){};
};

} // namespace Cool