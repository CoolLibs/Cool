#include "WindowFactoryU.h"
#include <Cool/Path/Path.h>
#include "img/src/Load.h"

namespace Cool::WindowFactoryU {

auto make_window_with_glfw(WindowConfig const& config, WindowManager& window_manager, GLFWwindow* window_to_share_opengl_context_with) -> Window&
{
    window_manager.windows().emplace_back(
        glfwCreateWindow(config.initial_width, config.initial_height, config.title, nullptr, window_to_share_opengl_context_with)
    );
    auto& window = window_manager.windows().back();
    if (!window.glfw())
    {
        const char* error_description; // NOLINT(*-init-variables)
        glfwGetError(&error_description);
        throw std::runtime_error{fmt::format("Window creation failed:\n{}", error_description)};
    }

    set_window_icon(window.glfw());

    return window;
}

void set_window_icon(GLFWwindow* window)
{
#ifdef COOL_APP_ICON_FILE // Don't do anything if no icon has been set
    auto icon  = *img::load(Cool::Path::root() / COOL_APP_ICON_FILE, 4, img::FirstRowIs::Top);
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

void apply_config(WindowConfig const& config, Window& window)
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
