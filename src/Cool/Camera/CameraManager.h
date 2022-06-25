#pragma once

#include <Cool/Camera/ViewController_Orbital.h>
#include <Cool/Constants/Constants.h>
#include <Cool/Input/MouseCoordinates.h>
#include <Cool/Input/MouveEventDispatcher.h>
#include "Camera.h"

namespace Cool {

class CameraManager {
public:
    explicit CameraManager(const glm::vec3& position = Constants::default_camera_position, const glm::vec3& look_at = glm::vec3{0.f});

    void hook_events(MouveEventDispatcher<ViewCoordinates>& dispatcher);
    auto inverse_view_projection_matrix(float aspect_ratio) const -> glm::mat4;
    void reset_transform();
    void imgui();

    const Camera& operator*() const { return _camera; }
    const Camera* operator->() const { return &operator*(); }
    Camera&       operator*() { return _camera; }
    Camera*       operator->() { return &operator*(); }

private:
    Camera                 _camera;
    ViewController_Orbital _view_controller;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Camera", _camera),
            cereal::make_nvp("ViewController", _view_controller)
        );
    }
};

} // namespace Cool