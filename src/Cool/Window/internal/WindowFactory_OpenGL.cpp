#if defined(COOL_OPENGL)

#include "WindowFactory_OpenGL.h"
#include <Cool/Utils/Version.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include "GLDebugCallback.h"
#include "WindowFactoryU.h"

namespace Cool {

WindowFactory_OpenGL::WindowFactory_OpenGL()
{
    static_assert(COOL_OPENGL_VERSION >= 330 && "ImGui requires at least OpenGL 3.3");
    Log::info("[Gpu] Using OpenGL {}.{}", major_version(COOL_OPENGL_VERSION), minor_version(COOL_OPENGL_VERSION));
}

void WindowFactory_OpenGL::shut_down(WindowManager& window_manager)
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    window_manager.windows().clear();
    glfwTerminate();
}

void WindowFactory_OpenGL::setup_main_window(Window_OpenGL& window)
{
    setupGLDebugging();
    setup_imgui(window);
}

void WindowFactory_OpenGL::setup_secondary_window(Window_OpenGL&, WindowManager& window_manager)
{
    window_manager.main_window().make_current();
}

Window_OpenGL& WindowFactory_OpenGL::make_window(const WindowConfig& config, WindowManager& window_manager)
{
    // Window flags
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major_version(COOL_OPENGL_VERSION));
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor_version(COOL_OPENGL_VERSION));
#if DEBUG
    if (COOL_OPENGL_VERSION >= 430) {
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    }
#endif
#if defined(__APPLE__)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    // Create window
    auto&       windows      = window_manager.windows();
    GLFWwindow* other_window = windows.empty() ? nullptr : windows.back().glfw();
    auto&       window       = WindowFactoryU::make_window_with_glfw<Window_OpenGL>(config, window_manager, other_window);
    window.make_current();
    WindowFactoryU::apply_config(config, window);
    // Load Glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { // NOLINT
        Log::error("Failed to initialize Glad");
    }
    //
    return window;
}

void WindowFactory_OpenGL::setupGLDebugging()
{
#if DEBUG
    if (COOL_OPENGL_VERSION >= 430) {
        int flags; // NOLINT
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(GLDebugCallback, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        }
        else {
            Log::warn("Couldn't setup OpenGL Debugging");
        }
    }
#endif
}

void WindowFactory_OpenGL::setup_imgui(Window_OpenGL& window)
{
    ImGui_ImplGlfw_InitForOpenGL(window.glfw(), false);
    std::string glslVersion = "#version " + std::to_string(COOL_OPENGL_VERSION);
    ImGui_ImplOpenGL3_Init(glslVersion.c_str());
}

} // namespace Cool

#endif