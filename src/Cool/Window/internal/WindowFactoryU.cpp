#include "WindowFactoryU.h"
#include "Cool/Path/Path.h"
#include "Cool/Utils/overloaded.hpp"

namespace Cool::WindowFactoryU {

static auto size_in_pixels(WindowSize size, int screen_size) -> int
{
    return std::visit(
        Cool::overloaded{
            [&](SizeProportionalToScreen sz) {
                return static_cast<int>(static_cast<float>(screen_size) * sz.proportion);
            },
            [&](SizeInPixels sz) {
                return sz.pixels;
            }
        },
        size
    );
}

auto make_window_with_glfw(WindowConfig const& config, WindowManager& window_manager, GLFWwindow* window_to_share_opengl_context_with) -> Window&
{
    GLFWvidmode const* const video_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    int const window_width  = size_in_pixels(config.initial_width, video_mode->width);
    int const window_height = size_in_pixels(config.initial_height, video_mode->height);

    window_manager.windows().emplace_back(
        glfwCreateWindow(window_width, window_height, config.title.c_str(), nullptr, window_to_share_opengl_context_with)
    );
    auto& window = window_manager.windows().back();
    if (!window.glfw())
    {
        const char* error_description; // NOLINT(*-init-variables)
        glfwGetError(&error_description);
        Log::internal_error("Window creation failed", error_description);
        throw std::runtime_error{fmt::format("Window creation failed:\n{}", error_description)};
    }

    // Center the window
    glfwSetWindowPos(window.glfw(), (video_mode->width - window_width) / 2, (video_mode->height - window_height) / 2);

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
