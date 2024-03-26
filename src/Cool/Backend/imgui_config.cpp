#include "imgui_config.h"
#include "Cool/ImGui/Fonts.h"
#include "Cool/ImGui/IcoMoonCodepoints.h"
#include "Cool/Path/Path.h"

namespace Cool {

static void set_imgui_ini_filepath()
{
    static std::string const path = (Cool::Path::user_data() / "imgui.ini").string(); // Needs to be static to keep the char* passed to `IniFilename` alive
    ImGui::GetIO().IniFilename    = path.c_str();
}

static void imgui_load_fonts()
{
    ImGuiIO&               io                     = ImGui::GetIO();
    static constexpr float font_size              = 18.0f;
    static constexpr float window_title_font_size = 18.0f;
    static constexpr float icons_size             = 16.0f; // Our icons font (IcoMoon) renders best at a multiple of 16px
    static constexpr float info_icon_size         = 14.0f;

    auto const merge_icons_into_current_font = [&]() { // Merge icons into default font
        ImFontConfig config;
        config.MergeMode   = true;
        config.PixelSnapH  = true;
        config.GlyphOffset = ImVec2{-4.f, +1.f};
        // config.GlyphMinAdvanceX            = font_size;                                   // Use if you want to make the icon monospaced
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
        auto const path   = Cool::Path::cool_res() / "fonts/Satoshi/Fonts/Satoshi-Bold.otf";
        Font::bold()      = io.Fonts->AddFontFromFileTTF(path.string().c_str(), font_size);
        Font::heading_1() = io.Fonts->AddFontFromFileTTF(path.string().c_str(), font_size * 1.6f);
        Font::heading_2() = io.Fonts->AddFontFromFileTTF(path.string().c_str(), font_size * 1.3f);
        Font::heading_3() = io.Fonts->AddFontFromFileTTF(path.string().c_str(), font_size * 1.1f);
        // merge_icons_into_current_font(); // Not needed for now
    }
    { // Italic font
        auto const path = Cool::Path::cool_res() / "fonts/Satoshi/Fonts/Satoshi-Italic.otf";
        Font::italic()  = io.Fonts->AddFontFromFileTTF(path.string().c_str(), font_size);
        merge_icons_into_current_font();
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

void imgui_config()
{
    set_imgui_ini_filepath();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigDockingAlwaysTabBar               = true;
    ImGui::GetStyle().WindowMenuButtonPosition = ImGuiDir_Right;
#if !defined(COOL_UPDATE_APP_ON_SEPARATE_THREAD)        // Platform windows freeze if we are not rendering on the main thread (TODO(WebGPU) : need to investigate that bug ; it is probably coming directly from ImGui)
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Must be done here at creation of the App, otherwise we can't toggle it at runtime.
#endif
    imgui_load_fonts();
}

} // namespace Cool