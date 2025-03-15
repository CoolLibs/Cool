#include "WindowFactory.h"
#include <stdexcept>
#include "Cool/DebugOptions/DebugOptions.h"
#include "Cool/ImGui/Fonts.h"
#include "Cool/ImGui/IcoMoonCodepoints.h"
#include "Cool/Log/Log.hpp"
#include "Cool/Path/Path.h"
#include "Cool/UI Scale/need_to_rebuild_fonts.hpp"
#include "Cool/UI Scale/ui_scale.hpp"
#include "Cool/UserSettings/UserSettings.h"
#include "GLFW/glfw3.h"
#include "ImGuiNotify/ImGuiNotify.hpp"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/imgui.h"

namespace Cool {

static void glfw_error_callback(int error, const char* description)
{
    Log::internal_error("glfw", fmt::format("Error {}: {}", error, description));
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
    static std::string const path = (Cool::Path::user_data() / "imgui.ini").string(); // Needs to be static to keep the char* passed to `IniFilename` alive
    ImGui::GetIO().IniFilename    = path.c_str();
}

void imgui_build_fonts_ifn()
{
    if (!need_to_rebuild_fonts())
        return;
    need_to_rebuild_fonts() = false;

    static auto previous_ui_scale{0.f};
    if (previous_ui_scale == ui_scale()) // Each time we reload the user_settings we set need_to_rebuild_fonts() to true, even if the ui scale hasn't actually changed. So we check this here, because rebuilding the fonts is very slow and we don't want to do it unless absolutely necessary
        return;
    previous_ui_scale = ui_scale();

    if (Cool::DebugOptions::log_when_building_font_atlas())
        Cool::Log::info("Font", "(Re)built font atlas");

    float const font_size              = std::round(16.0f * ui_scale());
    float const window_title_font_size = std::round(16.0f * ui_scale());
    float const icons_size             = std::round(13.0f * ui_scale()); // NB: Our icons font (IcoMoon) renders best at a multiple of 16px. But we cannot guarantee that size because users can scale the fonts to whatever they want
    float const info_icon_size         = std::round(13.0f * ui_scale());

    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();

    auto const merge_icons_into_current_font = [&]() { // Merge icons into default font
        ImFontConfig config;
        config.MergeMode   = true;
        config.PixelSnapH  = true;
        config.GlyphOffset = ImVec2{-4.f, +1.f};
        // config.GlyphMinAdvanceX = icons_size; // Use if you want to make the icons monospaced
        static const ImWchar icon_ranges[]          = {BEGIN_RANGE_ICOMOON, END_RANGE_ICOMOON, 0};     // NOLINT(*-avoid-c-arrays)
        static const ImWchar icon_ranges_for_info[] = {INFO_RANGE_ICOMOON, INFO_RANGE_ICOMOON + 1, 0}; // NOLINT(*-avoid-c-arrays)

        io.Fonts->AddFontFromFileTTF(
            (Cool::Path::cool_res() / "fonts/IcoMoon-Free/IcoMoon-Free.ttf").string().c_str(),
            info_icon_size, &config, icon_ranges_for_info
        );
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
        merge_icons_into_current_font();
        Font::heading_1() = io.Fonts->AddFontFromFileTTF(path.string().c_str(), font_size * 1.6f);
        Font::heading_2() = io.Fonts->AddFontFromFileTTF(path.string().c_str(), font_size * 1.3f);
        Font::heading_3() = io.Fonts->AddFontFromFileTTF(path.string().c_str(), font_size * 1.1f);
    }
    { // Italic font
        auto const path = Cool::Path::cool_res() / "fonts/Satoshi/Fonts/Satoshi-Italic.otf";
        Font::italic()  = io.Fonts->AddFontFromFileTTF(path.string().c_str(), font_size);
        merge_icons_into_current_font();
    }
    { // Monospace font
        auto const path   = Cool::Path::cool_res() / "fonts/Roboto_Mono/RobotoMono-Regular.ttf";
        Font::monospace() = io.Fonts->AddFontFromFileTTF(path.string().c_str(), font_size);
    }
    { // Regular font
        auto const path = Cool::Path::cool_res() / "fonts/Satoshi/Fonts/Satoshi-Regular.otf";
        Font::regular() = io.Fonts->AddFontFromFileTTF(path.string().c_str(), font_size);
        merge_icons_into_current_font();
        ImGuiNotify::add_icons_to_current_font(icons_size, ImVec2{0.f, 0.f});
    }

    io.Fonts->Build();
    ImGui_ImplOpenGL3_DestroyFontsTexture();
    ImGui_ImplOpenGL3_CreateFontsTexture();
}

static void initialize_imgui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    set_imgui_ini_filepath();
    // Load default settings first. All the new windows that are not in UserData's imgui.ini will get their values from here
    ImGui::LoadIniSettingsFromDisk((Cool::Path::default_user_data() / "imgui.ini").string().c_str());
    // Load user-defined settings second. They will override all the windows that were already known by the user, and which they might have customized
    ImGui::LoadIniSettingsFromDisk((Cool::Path::user_data() / "imgui.ini").string().c_str());

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigDockingAlwaysTabBar               = true;
    ImGui::GetStyle().WindowMenuButtonPosition = ImGuiDir_Right;
#if !defined(COOL_UPDATE_APP_ON_SEPARATE_THREAD)        // Platform windows freeze if we are not rendering on the main thread (TODO(JF) : need to investigate that bug ; it is probably coming directly from ImGui)
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Must be done here at creation of the App, otherwise we can't toggle it at runtime.
#endif
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