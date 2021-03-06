#include <Cool/AppManager/should_we_use_a_separate_thread_for_update.h>
#include <Cool/Log/ToUser.h>
#include <GLFW/glfw3.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/imgui.h>

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
    T::shut_down(_window_manager);
}

template<typename T>
void WindowFactory_Base<T>::glfw_error_callback(int error, const char* description)
{
    Log::error_without_breakpoint("[Glfw] Error {} :\n{}", error, description);
}

template<typename T>
Window& WindowFactory_Base<T>::make_main_window(const WindowConfig& config)
{
    auto& window = _impl.make_window(config, _window_manager);
    _window_manager.set_main_window(window);
    _impl.setup_main_window(window);
    return window;
}

template<typename T>
Window& WindowFactory_Base<T>::make_secondary_window(const WindowConfig& config)
{
    auto& window = _impl.make_window(config, _window_manager);
    T::setup_secondary_window(window, _window_manager);
    return window;
}

template<typename T>
void WindowFactory_Base<T>::initialize_glfw()
{
    glfwSetErrorCallback(WindowFactory_Base::glfw_error_callback);
    if (!glfwInit())
    {
        const char* error_description;
        glfwGetError(&error_description);
        Log::error("[WindowFactory_Base::initialize_glfw] Initialization failed :\n{}", error_description);
    }
}

static void set_imgui_ini_filepath()
{
    static const std::string path = Cool::Path::root() + "/imgui.ini"; // Needs to be static to keep the char* passed to io.IniFilename alive

    ImGui::GetIO().IniFilename = path.c_str();
}

template<typename T>
void WindowFactory_Base<T>::initialize_imgui()
{
    // Setup context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    set_imgui_ini_filepath();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
#if !defined(COOL_UPDATE_APP_ON_SEPARATE_THREAD)          // Platform windows freeze if we are not rendering on the main thread (TODO(JF) : need to investigate that bug ; it is probably comming directly from ImGui)
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
#endif
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
    // io.ConfigViewportsNoAutoMerge = true;
    // io.ConfigViewportsNoTaskBarIcon = true;

    // Setup style
    ImGui::StyleColorsClassic();
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding              = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
}

} // namespace Cool