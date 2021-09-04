#if defined(__COOL_APP_OPENGL)

#include "WindowFactory_OpenGL.h"
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include "GLDebugCallback.h"

namespace Cool {

WindowFactory_OpenGL::WindowFactory_OpenGL()
    : _opengl_version(COOL_OPENGL_VERSION)
    , _opengl_major_version(COOL_OPENGL_VERSION / 100)
    , _opengl_minor_version((COOL_OPENGL_VERSION - COOL_OPENGL_VERSION / 100 * 100) / 10)
{
    assert(_opengl_version >= 330);
    Log::info("[Gpu] Using OpenGL {}.{}", _opengl_major_version, _opengl_minor_version);
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

void WindowFactory_OpenGL::setup_secondary_window(Window_OpenGL& window, WindowManager& window_manager)
{
    window_manager.main_window().make_current();
}

Window_OpenGL& WindowFactory_OpenGL::make_window(const WindowCreationParams& params, WindowManager& window_manager)
{
    // Window flags
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, _opengl_major_version);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, _opengl_minor_version);
#ifdef DEBUG
    if (_opengl_version >= 430)
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    // Create window
    auto&       windows      = window_manager.windows();
    GLFWwindow* other_window = windows.empty() ? nullptr : windows.back().glfw();
    windows.push_back(Window_OpenGL{glfwCreateWindow(params.width, params.height, params.name, nullptr, other_window)});
    Window& window = windows.back();
    if (!window.glfw()) {
        const char* errorDescription;
        glfwGetError(&errorDescription);
        Log::error("[Glfw] Window or OpenGL context creation failed :\n{}", errorDescription);
    }
    window.make_current();
    window.cap_framerate(params.cap_framerate);
    // Load Glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        Log::error("Failed to initialize Glad");
    //
    return window;
}

void WindowFactory_OpenGL::setupGLDebugging()
{
#if defined(DEBUG)
    if (_opengl_version >= 430) {
        int flags;
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
    std::string glslVersion = "#version " + std::to_string(_opengl_version);
    ImGui_ImplOpenGL3_Init(glslVersion.c_str());
}

} // namespace Cool

#endif