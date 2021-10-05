#pragma once

#include <Cool/Camera/ProjectionController_Perspective.h>
#include <Cool/Camera/ViewController_Orbital.h>
#include <Cool/Input/MouseCoordinates.h>
#include <Cool/Input/MouveEventDispatcher.h>
#include "Camera.h"

namespace Cool {

class CameraManager {
public:
    CameraManager();

    void hook_events(MouveEventDispatcher<ViewCoordinates>& dispatcher);
    void update(float aspect_ratio);
    void imgui();

    const Camera& operator*() const { return _camera; }
    const Camera* operator->() const { return &operator*(); }

private:
    Camera                           _camera{{15.f, 0.f, 0.f}};
    ViewController_Orbital           _view_controller;
    ProjectionController_Perspective _projection_controller;
};

} // namespace Cool