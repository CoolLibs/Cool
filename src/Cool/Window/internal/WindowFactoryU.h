#pragma once
#include "../Window.h"
#include "../WindowManager.h"
#include "WindowConfig.h"

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
        Log::Debug::error("[glfw] Window creation failed:\n{}", error_description);
    }
    return window;
}

void apply_config(const WindowConfig& config, Window& window);

} // namespace Cool::WindowFactoryU
