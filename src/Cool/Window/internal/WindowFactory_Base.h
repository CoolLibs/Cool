#pragma once

namespace Cool {
/**
 * @brief A wrapper for Glfw + Vulkan/OpenGL + ImGui initialization and shutdown and windows creation
 * 
 */
class WindowFactory_Base {
public:
    WindowFactory_Base();

private:
    void        initializeGLFW();
    void        setup_imgui();
    static void GlfwErrorCallback(int error, const char* description);
};

} // namespace Cool