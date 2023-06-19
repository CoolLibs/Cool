#include "hook_events.h"

namespace Cool::CameraU {

void hook_events(MouseEventDispatcher<ViewCoordinates>& dispatcher, ViewController_Orbital& controller, Camera& camera)
{
    dispatcher
        .scroll_event()
        .subscribe([&](const auto& event) {
            controller.on_wheel_scroll(camera, event.dy);
        });
    dispatcher
        .drag()
        .start()
        .subscribe([&](const auto&) {
            controller.on_drag_start(camera);
        });
    dispatcher
        .drag()
        .update()
        .subscribe([&](const auto& event) {
            controller.on_drag(camera, event.delta);
        });
    dispatcher
        .drag()
        .stop()
        .subscribe([&](const auto&) {
            controller.on_drag_stop(camera);
        });
}

} // namespace Cool::CameraU
