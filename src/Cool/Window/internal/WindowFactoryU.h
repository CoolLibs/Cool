#pragma once
#include <Cool/Path/Path.h>
#include "../Window.h"
#include "../WindowManager.h"
#include "WindowConfig.h"
#include "img/src/Load.h"

namespace Cool::WindowFactoryU {

template<typename Window_Impl>
Window& make_window_with_glfw(const WindowConfig& config, WindowManager& window_manager, GLFWwindow* window_to_share_opengl_context_with = nullptr)
{
    window_manager.windows().push_back(Window_Impl{
        glfwCreateWindow(config.initial_width, config.initial_height, config.title, nullptr, window_to_share_opengl_context_with)});
    auto& window = window_manager.windows().back();
    if (!window.glfw())
    {
        const char* error_description; // NOLINT
        glfwGetError(&error_description);
        Log::Debug::error("glfw", fmt::format("Window creation failed:\n{}", error_description));
    }
    // TODO(WG) First check that COOL_APP_ICON_FILE has been set
    auto      icon = img::load(Cool::Path::root() / COOL_APP_ICON_FILE, 4, false);
    GLFWimage image;
    image.width  = icon.width();
    image.height = icon.height();
    image.pixels = icon.data();
    glfwSetWindowIcon(window.glfw(), 1, &image);
    return window;
}

void apply_config(const WindowConfig& config, Window& window);

} // namespace Cool::WindowFactoryU
