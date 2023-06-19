#include "hook_events.h"
#include <imgui.h>
#include "Cool/Input/MouseCoordinates.h"

namespace Cool::CameraU {

void hook_events(MouseEventDispatcher<ViewCoordinates>& dispatcher, ViewController_Orbital& controller, Camera& camera)
{
    dispatcher
        .scroll_event()
        .subscribe([&](auto const& event) {
            controller.on_wheel_scroll(camera, event.dy);
        });
    dispatcher
        .drag()
        .start()
        .subscribe([&](auto const&) {
            controller.on_drag_start(camera);
        });
    dispatcher
        .drag()
        .update()
        .subscribe([&](auto const&) {
            controller.on_drag(camera, ImGuiCoordinates{ImGui::GetIO().MouseDelta}); // NB: we don't use event.delta as it is in relative coordinates, and we want a delta in pixels to keep the drag speed the same no matter the size of the View.
        });
    dispatcher
        .drag()
        .stop()
        .subscribe([&](auto const&) {
            controller.on_drag_stop(camera);
        });
}

} // namespace Cool::CameraU
