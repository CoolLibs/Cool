#include "CameraManager.h"
#include <Cool/Camera/hook_events.h>

namespace Cool {

CameraManager::CameraManager()
{
    _view_controller.set_distance_to_orbit_center(15.f);
}

void CameraManager::hook_events(MouveEventDispatcher<ViewCoordinates>& dispatcher)
{
    CameraU::hook_events(dispatcher, _view_controller, _camera);
}

void CameraManager::update(float aspect_ratio)
{
    _projection_controller.apply_to(_camera, aspect_ratio);
}

void CameraManager::imgui()
{
    _view_controller.ImGui();
    if (ImGui::Button("Look at the origin")) {
        _view_controller.set_orbit_center({0, 0, 0}, _camera);
    }
    _projection_controller.ImGui();
}

} // namespace Cool