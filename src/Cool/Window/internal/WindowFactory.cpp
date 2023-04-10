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
    static constexpr float font_size              = 17.0f;
    static constexpr float window_title_font_size = 19.0f;
    static constexpr float icons_size             = 16.0f; // Our icons font (IcoMoon) renders best at a multiple of 16px

    auto const merge_icons_into_current_font = [&]() { // Merge icons into default font
        ImFontConfig config;
        config.MergeMode  = true;
        config.PixelSnapH = true;
        config.GlyphOffset = ImVec2{-4.f, +1.f};
        // config.GlyphMinAdvanceX            = font_size;                                   // Use if you want to make the icon monospaced
        static const ImWchar icon_ranges[] = {BEGIN_RANGE_ICOMOON, END_RANGE_ICOMOON, 0}; // NOLINT(*-avoid-c-arrays)
        io.Fonts->AddFontFromFileTTF(
            (Cool::Path::cool_res() / "fonts/IcoMoon-Free/IcoMoon-Free.ttf").string().c_str(),
            icons_size, &config, icon_ranges
        );
    };

    { // Window title font // Must be first so that it is the default font. This is mandatory because window titles can only use the default font.

        auto const path      = Cool::Path::cool_res() / "fonts/Satoshi/Fonts/Satoshi-Bold.otf";
        Font::window_title() = io.Fonts->AddFontFromFileTTF(path.string().c_str(), window_title_font_size);
        merge_icons_into_current_font();
    }
    { // Bold font
        auto const path = Cool::Path::cool_res() / "fonts/Satoshi/Fonts/Satoshi-Bold.otf";
        Font::bold()    = io.Fonts->AddFontFromFileTTF(path.string().c_str(), font_size);
        // merge_icons_into_current_font(); // Not needed for now
    }
    { // Regular font
        auto const path = Cool::Path::cool_res() / "fonts/Satoshi/Fonts/Satoshi-Regular.otf";
        Font::regular() = io.Fonts->AddFontFromFileTTF(path.string().c_str(), font_size);
        merge_icons_into_current_font();
    }
    { // Monospace font
        auto const path   = Cool::Path::cool_res() / "fonts/Roboto_Mono/RobotoMono-Regular.ttf";
        Font::monospace() = io.Fonts->AddFontFromFileTTF(path.string().c_str(), font_size);
    }

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
    io.ConfigWindowsMoveFromTitleBarOnly       = true; // Required because when we move a camera in a View we don't want to also move the window.
    io.ConfigDockingAlwaysTabBar               = true;
    io.ConfigDragClickToInputText              = true;
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