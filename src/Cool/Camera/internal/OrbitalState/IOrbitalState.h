#pragma once
#include "Cool/Input/MouseCoordinates.h"

namespace Cool {

class ViewController_Orbital;
class Camera;

class IOrbitalState {
public:
    virtual auto on_drag(ViewController_Orbital& /*controller*/, Camera& /*camera*/, ImGuiCoordinates const& /*delta*/) -> bool { return false; }
    virtual auto on_wheel_scroll(ViewController_Orbital& /*controller*/, Camera& /*camera*/, float /*dl*/) -> bool { return false; }
    virtual auto on_drag_start(ViewController_Orbital& /*controller*/, Camera& /*camera*/) -> bool { return false; }
    virtual auto on_drag_stop(ViewController_Orbital& /*controller*/, Camera& /*camera*/) -> bool { return false; }
};

} // namespace Cool