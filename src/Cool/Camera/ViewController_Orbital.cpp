#include "ViewController_Orbital.h"
#include "Camera.h"

namespace Cool {

ViewController_Orbital::ViewController_Orbital(ViewController_Orbital::Mode mode)
    : _mode{mode}
{
}

bool ViewController_Orbital::ImGui()
{
    bool b                  = false;
    int  current_combo_item = [&]() {
        switch (_mode) {
        case ViewController_Orbital::Mode::Turntable:
            return 0;
        case ViewController_Orbital::Mode::Trackball:
            return 1;
        case ViewController_Orbital::Mode::AxisFree:
            return 2;
        default:
            Log::error("[ViewController_Orbital::ImGui] Unknown ViewController_Orbital::Mode");
            return 0;
        }
    }();
    if (b = ImGui::Combo("Orbit Mode", &current_combo_item, " Turntable\0 Trackball\0 AxisFree\0\0")) {
        switch (current_combo_item) {
        case 0:
            _mode = ViewController_Orbital::Mode::Turntable;
            break;
        case 1:
            _mode = ViewController_Orbital::Mode::Trackball;
            break;
        case 2:
            _mode = ViewController_Orbital::Mode::AxisFree;
            break;
        }
    }
    return b;
}

void ViewController_Orbital::set_orbit_center(glm::vec3 const& orbit_center, Camera& camera)
{
    camera.translate(orbit_center - get_orbit_center(camera));
}

glm::vec3 ViewController_Orbital::get_orbit_center(Camera const& camera) const
{
    return camera.position() + _distance_to_orbit_center * camera.front_axis();
}

} // namespace Cool