#include "CameraManager.h"
#include <Cool/Camera/hook_events.h>

namespace Cool {

CameraManager::CameraManager(const glm::vec3& position, const glm::vec3& look_at)
    : _camera{position, look_at}
{
    _view_controller.set_distance_to_orbit_center(glm::distance(position, look_at));
}

void CameraManager::hook_events(MouveEventDispatcher<ViewCoordinates>& dispatcher)
{
    CameraU::hook_events(dispatcher, _view_controller, _camera);
}

void CameraManager::apply(float aspect_ratio)
{
    _projection_controller.apply_to(_camera, aspect_ratio);
}

void CameraManager::reset_transform()
{
    const auto position = glm::vec3{15.f, 0.f, 0.f};
    const auto look_at  = glm::vec3{0.f};

    _camera = Camera{position, look_at};
    _view_controller.set_distance_to_orbit_center(glm::distance(position, look_at));
}

void CameraManager::imgui()
{
    _view_controller.ImGui();
    if (ImGui::Button("Look at the origin")) {
        _view_controller.set_orbit_center({0, 0, 0}, _camera);
    }
    if (ImGui::Button("Reset transform")) {
        reset_transform();
    }
    _projection_controller.ImGui();
}

} // namespace Cool