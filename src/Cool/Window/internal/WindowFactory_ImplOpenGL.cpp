#if defined(COOL_OPENGL)

#include "WindowFactory_ImplOpenGL.h" // Must be included first
//
#include <Cool/Utils/Version.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include "GLDebugCallback.h"
#include "WindowFactoryU.h"

namespace Cool {

WindowFactory_ImplOpenGL::WindowFactory_ImplOpenGL()
{
    static_assert(COOL_OPENGL_VERSION >= 330 && "ImGui requires at least OpenGL 3.3");
}

void WindowFactory_ImplOpenGL::shut_down(WindowManager& window_manager)
{
    shutdown_imgui();
    ImGui::DestroyContext();
    window_manager.windows().clear();
    glfwTerminate();
#if DEBUG
    CoolGlDebug::shut_down();
#endif
}

void WindowFactory_ImplOpenGL::setup_main_window(Window_OpenGL& window)
{
    setupGLDebugging();
    setup_imgui(window);
}

void WindowFactory_ImplOpenGL::setup_secondary_window(Window_OpenGL&, WindowManager& window_manager)
{
    window_manager.main_window().make_current();
}

Window_OpenGL& WindowFactory_ImplOpenGL::make_window(const WindowConfig& config, WindowManager& window_manager)
{
    // Window flags
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major_version(COOL_OPENGL_VERSION));
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor_version(COOL_OPENGL_VERSION));
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#if DEBUG
    if (COOL_OPENGL_VERSION >= 430)
    {
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    }
#endif
    // Create window
    auto&       windows      = window_manager.windows();
    GLFWwindow* other_window = windows.empty() ? nullptr : windows.back().glfw();
    auto&       window       = WindowFactoryU::make_window_with_glfw<Window_OpenGL>(config, window_manager, other_window);
    window.make_current();
    WindowFactoryU::apply_config(config, window);
    // Load Glad
    if (!gladLoadGL(glfwGetProcAddress)) // NOLINT
    {
        Log::Debug::error("WindowFactory_ImplOpenGL::make_window", "Failed to initialize Glad");
    }
    //
    return window;
}

void WindowFactory_ImplOpenGL::setupGLDebugging()
{
#if DEBUG
    if (COOL_OPENGL_VERSION >= 430)
    {
        int flags; // NOLINT
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
        {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(GLDebugCallback, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        }
        else
        {
            Log::Debug::warning(
                "WindowFactory_ImplOpenGL::setupGLDebugging",
                "Couldn't setup OpenGL Debugging"
            );
        }
    }
#endif
}

void WindowFactory_ImplOpenGL::setup_imgui(Window_OpenGL& window)
{
    ImGui_ImplGlfw_InitForOpenGL(window.glfw(), false);
    std::string glslVersion = "#version " + std::to_string(COOL_OPENGL_VERSION);
    ImGui_ImplOpenGL3_Init(glslVersion.c_str());
}

void WindowFactory_ImplOpenGL::shutdown_imgui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
}

} // namespace Cool

#endif