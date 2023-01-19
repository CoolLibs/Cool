#include "WindowFactoryU.h"
#include <Cool/Path/Path.h>
#include "img/src/Load.h"

namespace Cool::WindowFactoryU {

void set_window_icon(GLFWwindow* window)
{
#ifdef COOL_APP_ICON_FILE // Don't do anything if no icon has been set
    auto icon  = img::load(Cool::Path::root() / COOL_APP_ICON_FILE, 4, false);
    auto image = GLFWimage{
        .width  = static_cast<int>(icon.width()),
        .height = static_cast<int>(icon.height()),
        .pixels = icon.data(),
    };
    glfwSetWindowIcon(window, 1, &image);
#else
    (void)window; // Prevent warning
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
