#include "CameraManager.h"
#include <Cool/Camera/hook_events.h>
#include "ViewController_OrbitalU.h"

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
    ViewController_OrbitalU::reset_transform(_view_controller, _camera);
}

void CameraManager::imgui()
{
    _view_controller.ImGui(_camera);
    if (ImGui::Button("Look at the origin")) {
        _view_controller.set_orbit_center({0, 0, 0}, _camera);
    }
    if (ImGui::Button("Reset transform")) {
        reset_transform();
    }
    _projection_controller.ImGui();
}

} // namespace Cool