#include "WindowFactoryU.h"

namespace Cool::WindowFactoryU {

void apply_config(const WindowConfig& config, Window& window)
{
    window.cap_framerate_if(config.cap_framerate_on_startup_if);
    if (config.maximize_on_startup_if)
    {
        glfwMaximizeWindow(window.glfw());
    }
    if (config.hide_on_startup_if)
    {
        window.set_visibility(false);
    }
}

} // namespace Cool::WindowFactoryU
