#include "HookEvents.h"

namespace Cool {

void hook_events(MouveEventDispatcher<ImGuiWindowCoordinates>& dispatcher, ViewController_Orbital& controller, Camera& camera)
{
    dispatcher
        .scroll_event()
        .subscribe([&](const auto& event) {
            controller.on_wheel_scroll(camera, event.dy);
        });
    dispatcher
        .drag()
        .start()
        .subscribe([&](const auto& event) {
            controller.on_wheel_down(camera, event.mods);
        });
    dispatcher
        .drag()
        .update()
        .subscribe([&](const auto& event) {
            controller.on_mouse_move(camera, event.delta);
        });
    dispatcher
        .drag()
        .stop()
        .subscribe([&](const auto& event) {
            controller.on_wheel_up(camera);
        });
}

} // namespace Cool
