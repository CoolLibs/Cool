#pragma once

#include <Cool/Camera/ProjectionController_Perspective.h>
#include <Cool/Camera/ViewController_Orbital.h>
#include <Cool/Input/MouseCoordinates.h>
#include <Cool/Input/MouveEventDispatcher.h>
#include "Camera.h"

namespace Cool {

class CameraManager {
public:
    explicit CameraManager(const glm::vec3& position = {15.f, 0.f, 0.f}, const glm::vec3& look_at = glm::vec3{0.f});

    void hook_events(MouveEventDispatcher<ViewCoordinates>& dispatcher);
    void apply(float aspect_ratio);
    void imgui();

    const Camera& operator*() const { return _camera; }
    const Camera* operator->() const { return &operator*(); }

private:
    Camera                           _camera;
    ViewController_Orbital           _view_controller;
    ProjectionController_Perspective _projection_controller;
};

} // namespace Cool