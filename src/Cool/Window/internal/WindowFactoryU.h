#pragma once
#include <stdexcept>
#include "../Window.h"
#include "../WindowManager.h"
#include "WindowConfig.h"

namespace Cool::WindowFactoryU {

[[nodiscard]] auto make_window_with_glfw(WindowConfig const& config, WindowManager& window_manager, GLFWwindow* window_to_share_opengl_context_with = nullptr) -> Window&;
void               set_window_icon(GLFWwindow* window);
void               apply_config(const WindowConfig& config, Window& window);

} // namespace Cool::WindowFactoryU
