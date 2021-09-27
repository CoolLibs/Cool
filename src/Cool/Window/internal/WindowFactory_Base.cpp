#include "WindowFactory_Base.h"
#include <Cool/Log/ToUser.h>
#include <glfw/glfw3.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/imgui.h>

#if defined(__COOL_APP_VULKAN)
#include "WindowFactory_Vulkan.h"
template class Cool::WindowFactory_Base<Cool::WindowFactory_Vulkan>;
#elif defined(__COOL_APP_OPENGL)
#include "WindowFactory_OpenGL.h"
template class Cool::WindowFactory_Base<Cool::WindowFactory_OpenGL>;
#endif

namespace Cool {

template<typename T>
WindowFactory_Base<T>::WindowFactory_Base()
{
    initialize_glfw();
    initialize_imgui();
}

template<typename T>
WindowFactory_Base<T>::~WindowFactory_Base()
{
    _impl.shut_down(_window_manager);
}

template<typename T>
void WindowFactory_Base<T>::glfw_error_callback(int error, const char* description)
{
    Log::error("[Glfw] Error {} :\n{}", error, description);
}

template<typename T>
Window& WindowFactory_Base<T>::make_main_window(const WindowCreationParams& params)
{
    auto& window = _impl.make_window(params, _window_manager);
    _window_manager.set_main_window(window);
    _impl.setup_main_window(window);
    return window;
}

template<typename T>
Window& WindowFactory_Base<T>::make_secondary_window(const WindowCreationParams& params)
{
    auto& window = _impl.make_window(params, _window_manager);
    _impl.setup_secondary_window(window, _window_manager);
    return window;
}

template<typename T>
void WindowFactory_Base<T>::initialize_glfw()
{
    glfwSetErrorCallback(WindowFactory_Base::glfw_error_callback);
    if (!glfwInit()) {
        const char* error_description;
        glfwGetError(&error_description);
        Log::error("[WindowFactory_Base::initialize_glfw] Initialization failed :\n{}", error_description);
    }
}

template<typename T>
void WindowFactory_Base<T>::initialize_imgui()
{
    // Setup context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
#if defined(__COOL_APP_OPENGL)                            // DISABLED for Vulkan because platform windows freeze since we are not rendering on the main thread
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
#endif
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup style
    ImGui::StyleColorsClassic();
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding              = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
}

} // namespace Cool