#include "debug_options_windows.h"
#include "Cool/ImGui/ColorThemes.h"
#include "Cool/ImGui/Fonts.h"
#include "Cool/ImGui/imgui_show_all_icons.hpp"
#include "Cool/ImGui/test_markdown_formatting.h"
#include "Cool/Log/TestMessageConsole.h"
#include "Cool/Midi/MidiManager.h"
#include "Cool/TextureSource/TextureLibrary_Image.h"
#include "Cool/TextureSource/TextureLibrary_Video.h"
#include "Cool/Tips/test_tips.h"
#include "Cool/Variables/TestPresets.h"
#include "Cool/Variables/TestVariables.h"
#include "DebugOptions.h"
#include "TestNotification.hpp"
#include "TestTasks.hpp"

namespace Cool {

void debug_options_windows(TipsManager* tips_manager, Window& main_window)
{
    DebugOptions::show_framerate_window([&] {
        ImGui::PushFont(Cool::Font::monospace());
        ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
        auto const ms = 1000.f / ImGui::GetIO().Framerate;
        ImGui::Text(ms >= 10.f ? "%.2f ms" : "0%.2f ms", ms);
        ImGui::PopFont();
        main_window.imgui_cap_framerate();
    });

    if (DebugOptions::show_imgui_demo_window())                         // Show the big demo window (Most of the sample code is
        ImGui::ShowDemoWindow(&DebugOptions::show_imgui_demo_window()); // in ImGui::ShowDemoWindow()! You can browse its code
                                                                        // to learn more about Dear ImGui!).

    DebugOptions::test_all_variable_widgets__window(&test_variables);
    DebugOptions::empty_window([] {});

    DebugOptions::texture_library_debug_view([&] {
        ImGui::SeparatorText("Image");
        TextureLibrary_Image::instance().imgui_debug_view();
        ImGui::SeparatorText("Video");
        TextureLibrary_Video::instance().imgui_debug_view();
    });

    DebugOptions::test_message_console__window([]() {
        static auto test_message_console = TestMessageConsole{};
        test_message_console.imgui();
    });

    DebugOptions::test_notifications__window([]() {
        static auto test_notification = TestNotification{};
        test_notification.imgui();
    });

    DebugOptions::test_tasks__window([]() {
        static auto test_tasks = TestTasks{};
        test_tasks.imgui();
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

    if (tips_manager)
        DebugOptions::test_tips([&]() {
            test_tips(*tips_manager);
        });

    DebugOptions::color_themes_advanced_config_window([]() {
        color_themes()->imgui_advanced_config();
    });

#if defined(DEBUG)
    DebugOptions::show_all_icons([]() {
        imgui_show_all_icons();
    });
#endif

    DebugOptions::color_themes_editor([]() {
        color_themes()->imgui_basic_theme_editor();
    });
}

} // namespace Cool