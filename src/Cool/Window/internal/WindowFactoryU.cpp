#include "WindowFactoryU.h"

namespace Cool::WindowFactoryU {

void set_window_icon(GLFWwindow* window)
{
#ifdef COOL_APP_ICON_FILE
    auto      icon = img::load(Cool::Path::root() / COOL_APP_ICON_FILE, 4, false);
    GLFWimage image;
    image.width  = icon.width();
    image.height = icon.height();
    image.pixels = icon.data();
    glfwSetWindowIcon(window, 1, &image);
#endif
}

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
