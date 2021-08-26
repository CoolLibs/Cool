#pragma once

#include "WindowManager.h"

namespace Cool {
/**
 * @brief A wrapper for Glfw + Vulkan/OpenGL + ImGui initialization and shutdown and windows creation
 * 
 */
class WindowFactory_Base {
public:
    WindowFactory_Base();

    WindowManager& window_manager() { return _window_manager; }

private:
    void        initializeGLFW();
    void        initialize_imgui();
    static void GlfwErrorCallback(int error, const char* description);

private:
    WindowManager _window_manager;
};

} // namespace Cool