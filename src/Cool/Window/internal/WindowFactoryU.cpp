#include "WindowFactoryU.h"

namespace Cool::WindowFactoryU {

void apply_config(Window& window, WindowCreationParams config)
{
    window.cap_framerate(is_building_as(config.cap_framerate_if));
    if (is_building_as(config.maximize_on_startup_if)) {
        glfwMaximizeWindow(window.glfw());
    }
    if (is_building_as(config.hide_on_startup_if)) {
        window.set_visibility(false);
    }
}

} // namespace Cool::WindowFactoryU
