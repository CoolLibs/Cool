#include "debug_options_windows.h"
#include <Cool/ImGui/test_markdown_formatting.h>
#include <Cool/Log/TestMessageConsole.h>
#include <Cool/Midi/MidiManager.h>
#include <Cool/TextureSource/TextureLibrary_Image.h>
#include <Cool/TextureSource/TextureLibrary_Video.h>
#include <Cool/Tips/test_tips.h>
#include <Cool/UserSettings/UserSettings.h>
#include <Cool/Variables/TestPresets.h>
#include "Cool/ImGui/imgui_show_all_icons.hpp"
#include "DebugOptions.h"
#include "TestNotification.h"

namespace Cool {

void debug_options_windows(TipsManager& tips_manager)
{
    DebugOptions::texture_library_debug_view([&] {
        ImGui::SeparatorText("Image");
        TextureLibrary_Image::instance().imgui_debug_view();
        ImGui::SeparatorText("Video");
        TextureLibrary_Video::instance().imgui_debug_view();
    });

    DebugOptions::test_message_console__window([]() {
        static auto test_message_console = TestMessageConsole{};
        test_message_console.imgui(
            Log::ToUser::console()
        );
    });

    DebugOptions::test_notifications__window([]() {
        static auto test_notification = TestNotification{};
        test_notification.imgui();
    });

    DebugOptions::test_presets__window([]() {
        static auto test_presets = TestPresets{};
        test_presets.imgui();
    });

    DebugOptions::test_markdown_formatting_window([]() {
        test_markdown_formatting();
    });

    DebugOptions::emulate_midi_keyboard([]() {
        midi_manager().imgui_emulate_midi_keyboard();
    });

    DebugOptions::test_tips([&]() {
        test_tips(tips_manager);
    });

    DebugOptions::color_themes_advanced_config_window([]() {
        user_settings().color_themes.imgui_advanced_config();
    });

#if defined(DEBUG)
    DebugOptions::show_all_icons([]() {
        imgui_show_all_icons();
    });
#endif

    DebugOptions::color_themes_editor([]() {
        user_settings().color_themes.imgui_basic_theme_editor();
    });
}

} // namespace Cool