#if defined(__COOL_APP_OPENGL)

#include "WindowFactory_OpenGL.h"
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include "GLDebugCallback.h"

namespace Cool {

WindowFactory_OpenGL::WindowFactory_OpenGL(int openGLMajorVersion, int openGLMinorVersion)
    : m_openGLMajorVersion(openGLMajorVersion), m_openGLMinorVersion(openGLMinorVersion), m_openGLVersion(openGLMajorVersion * 100 + openGLMinorVersion * 10)
{
    assert(m_openGLVersion >= 330);
}

WindowFactory_OpenGL::~WindowFactory_OpenGL()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    window_manager().windows().clear();
    glfwTerminate();
}

Window_OpenGL& WindowFactory_OpenGL::make_main_window(const char* name, int width, int height, bool cap_framerate)
{
    auto& window = make_window(name, width, height, cap_framerate);
    setup_imgui(window);
    return window;
}

Window_OpenGL& WindowFactory_OpenGL::make_secondary_window(const char* name, int width, int height, bool cap_framerate)
{
    return make_window(name, width, height, cap_framerate);
}

Window_OpenGL& WindowFactory_OpenGL::make_window(const char* name, int width, int height, bool cap_framerate)
{
    // Window flags
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_openGLMajorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_openGLMinorVersion);
#ifdef DEBUG
    if (m_openGLVersion >= 430)
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    // Create window
    auto&       windows      = window_manager().windows();
    GLFWwindow* other_window = windows.empty() ? nullptr : windows.back().glfw();
    windows.push_back(Window_OpenGL{glfwCreateWindow(width, height, name, nullptr, other_window)});
    Window& window = windows.back();
    if (!window.glfw()) {
        const char* errorDescription;
        glfwGetError(&errorDescription);
        Log::error("[Glfw] Window or OpenGL context creation failed :\n{}", errorDescription);
    }
    window.make_current();
    window.cap_framerate(cap_framerate);
    // Load Glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        Log::error("Failed to initialize Glad");
    //
    setupGLDebugging();
    setup_imgui(window);
    return window;
}

void WindowFactory_OpenGL::setupGLDebugging()
{
#if defined(DEBUG)
    if (m_openGLVersion >= 430) {
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
    std::string glslVersion = "#version " + std::to_string(m_openGLVersion);
    ImGui_ImplOpenGL3_Init(glslVersion.c_str());
}

} // namespace Cool

#endif