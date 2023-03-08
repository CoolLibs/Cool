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
    Log::Debug::error_without_breakpoint("glfw", fmt::format("Error {}:\n{}", error, description));
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
        Log::Debug::error("WindowFactory_Base::initialize_glfw", fmt::format("Initialization failed:\n{}", error_description));
    }
}

static void set_imgui_ini_filepath()
{
    static const std::string path = (Cool::Path::root() / "imgui.ini").string(); // Needs to be static to keep the char* passed to io.IniFilename alive

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

    // Fonts
    // Merge icons into default tool font
    io.Fonts->AddFontDefault();
    float baseFontSize = 13.0f;        // 13.0f is the size of the default font. Change to the font size you use.
    float iconFontSize = baseFontSize; //* 2.0f / 3.0f; // FontAwesome fonts need to have their sizes reduced by 2.0f/3.0f in order to align correctly

    ImFontConfig config;
    config.MergeMode                       = true;
    config.PixelSnapH                      = true;
    config.GlyphMinAdvanceX                = iconFontSize; // Use if you want to make the icon monospaced
    static const ImWchar     icon_ranges[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
    ImVector<ImWchar>        ranges;
    ImFontGlyphRangesBuilder builder;
    // builder.AddText("Hello world");
    builder.AddText(ICON_FA_DIAGRAM_PROJECT);
    builder.BuildRanges(&ranges);
    io.Fonts->AddFontFromFileTTF((Cool::Path::cool_res() / "fonts/Font Awesome 6 Free-Solid-900.otf").string().c_str(), iconFontSize, &config, icon_ranges);
    io.Fonts->Build();

    // Setup style
    ImGui::StyleColorsClassic();
    ImGuiStyle& style                 = ImGui::GetStyle();
    style.WindowRounding              = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
}

} // namespace Cool