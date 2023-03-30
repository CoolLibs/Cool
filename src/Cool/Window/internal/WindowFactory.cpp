#include "WindowFactory.h"
#include <Cool/AppManager/should_we_use_a_separate_thread_for_update.h>
#include <Cool/Path/Path.h>
#include <GLFW/glfw3.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/imgui.h>
#include <filesystem>
#include <stdexcept>
#include "Cool/ImGui/Fonts.h"
#include "Cool/ImGui/IcoMoonCodepoints.h"

namespace Cool {

static void glfw_error_callback(int error, const char* description)
{
    Log::Debug::error_without_breakpoint("glfw", fmt::format("Error {}:\n{}", error, description));
}

static void initialize_glfw()
{
    glfwSetErrorCallback(&glfw_error_callback);
    if (!glfwInit())
    {
        const char* error_description; // NOLINT(*-init-variables)
        glfwGetError(&error_description);
        throw std::runtime_error{fmt::format("GLFW initialization failed:\n{}", error_description)};
    }
}

static void set_imgui_ini_filepath()
{
    static std::string const path = (Cool::Path::root() / "imgui.ini").string(); // Needs to be static to keep the char* passed to `IniFilename` alive
    ImGui::GetIO().IniFilename    = path.c_str();
}

static void imgui_load_fonts()
{
    ImGuiIO&               io        = ImGui::GetIO();
    static constexpr float font_size = 16.0f; // Our icons font renders best at a multiple of 16px

    { // Main font
        auto path = Cool::Path::cool_res() / "fonts/main_font.ttf";
        if (!std::filesystem::exists(path))
            path = Cool::Path::cool_res() / "fonts/main_font.otf";
        if (!std::filesystem::exists(path))
            io.Fonts->AddFontDefault();
        else
            io.Fonts->AddFontFromFileTTF(path.string().c_str(), font_size);
    }
    { // Merge icons into default font
        ImFontConfig config;
        config.MergeMode                   = true;
        config.PixelSnapH                  = true;
        config.GlyphMinAdvanceX            = font_size;                                   // Use if you want to make the icon monospaced
        static const ImWchar icon_ranges[] = {ICOMOON_RANGE_BEGIN, ICOMOON_RANGE_END, 0}; // NOLINT(*-avoid-c-arrays)
        io.Fonts->AddFontFromFileTTF(
            (Cool::Path::cool_res() / "fonts/IcoMoon-Free/IcoMoon-Free.ttf").string().c_str(),
            font_size, &config, icon_ranges
        );
    }

    // Console font
    Font::console() = io.Fonts->AddFontFromFileTTF((Cool::Path::cool_res() / "fonts/Roboto_Mono/RobotoMono-VariableFont_wght.ttf").string().c_str(), font_size);

    io.Fonts->Build();
}

static void initialize_imgui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    set_imgui_ini_filepath();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigDockingAlwaysTabBar  = true;
    io.ConfigDragClickToInputText = true;
    ImGui::GetStyle().WindowMenuButtonPosition = ImGuiDir_Right;
#if !defined(COOL_UPDATE_APP_ON_SEPARATE_THREAD)          // Platform windows freeze if we are not rendering on the main thread (TODO(JF) : need to investigate that bug ; it is probably coming directly from ImGui)
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
#endif
    imgui_load_fonts();
}

WindowFactory::WindowFactory()
{
    initialize_glfw();
    initialize_imgui();
}

WindowFactory::~WindowFactory()
{
    WindowFactory_Impl::shut_down(_window_manager);
}

auto WindowFactory::make_main_window(WindowConfig const& config) -> Window&
{
    auto& window = WindowFactory_Impl::make_window(config, _window_manager);
    WindowFactory_Impl::setup_main_window(window);
    _window_manager.set_main_window(window);
    return window;
}

auto WindowFactory::make_secondary_window(const WindowConfig& config) -> Window&
{
    auto& window = WindowFactory_Impl::make_window(config, _window_manager);
    WindowFactory_Impl::setup_secondary_window(window, _window_manager);
    return window;
}

} // namespace Cool