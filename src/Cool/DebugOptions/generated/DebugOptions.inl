/* -----------------------------------------------------------------------------
 * This file was automatically generated by a Python script.
 * PLEASE DON'T EDIT IT DIRECTLY, your changes would be overwritten the next time the script is run.
 * Instead, go to "generate_debug_options.py" and edit the "DebugOptions" function there.
 * -----------------------------------------------------------------------------
 */

#include <Cool/ImGui/ImGuiExtras.h>
#include <wafl/wafl.hpp>

namespace Cool {
template<typename... Ts>
class DebugOptionsManager; // Forward declare this class so that the friend declaration that comes later on doesn't fail.
}

namespace Cool {

class DebugOptions {
public:
    static void test_message_console__window(std::function<void()> callback)
    {
        if (instance().test_message_console__window)
        {
            ImGui::Begin(Cool::icon_fmt("Test Message Console", ICOMOON_WRENCH).c_str(), &instance().test_message_console__window, ImGuiWindowFlags_NoFocusOnAppearing);
            callback();
            ImGui::End();
        }
    }
    [[nodiscard]] static auto log_when_autosaving() -> bool& { return instance().log_when_autosaving; }
    [[nodiscard]] static auto log_when_rendering_alpha_checkerboard_background() -> bool& { return instance().log_when_rendering_alpha_checkerboard_background; }
    [[nodiscard]] static auto log_when_creating_textures() -> bool& { return instance().log_when_creating_textures; }
    static void               texture_library_debug_view(std::function<void()> callback)
    {
        if (instance().texture_library_debug_view)
        {
            ImGui::Begin(Cool::icon_fmt("Texture Library", ICOMOON_WRENCH).c_str(), &instance().texture_library_debug_view, ImGuiWindowFlags_NoFocusOnAppearing);
            callback();
            ImGui::End();
        }
    }
    [[nodiscard]] static auto log_number_of_threads_in_the_thread_pool() -> bool& { return instance().log_number_of_threads_in_the_thread_pool; }
#if DEBUG
    [[nodiscard]] static auto log_opengl_info() -> bool&
    {
        return instance().log_opengl_info;
    }
#endif
    static void test_presets__window(std::function<void()> callback)
    {
        if (instance().test_presets__window)
        {
            ImGui::Begin(Cool::icon_fmt("Test Presets", ICOMOON_WRENCH).c_str(), &instance().test_presets__window, ImGuiWindowFlags_NoFocusOnAppearing);
            callback();
            ImGui::End();
        }
    }
    static void test_markdown_formatting_window(std::function<void()> callback)
    {
        if (instance().test_markdown_formatting_window)
        {
            ImGui::Begin(Cool::icon_fmt("Test Markdown Formatting", ICOMOON_WRENCH).c_str(), &instance().test_markdown_formatting_window, ImGuiWindowFlags_NoFocusOnAppearing);
            callback();
            ImGui::End();
        }
    }
    static void test_did_you_know(std::function<void()> callback)
    {
        if (instance().test_did_you_know)
        {
            ImGui::Begin(Cool::icon_fmt("Test \"Did you know?\"", ICOMOON_WRENCH).c_str(), &instance().test_did_you_know, ImGuiWindowFlags_NoFocusOnAppearing);
            callback();
            ImGui::End();
        }
    }
    [[nodiscard]] static auto public_exhibition_mode() -> bool& { return instance().public_exhibition_mode; }
    static void               style_editor(std::function<void()> callback)
    {
        if (instance().style_editor)
        {
            ImGui::Begin(Cool::icon_fmt("Style Editor", ICOMOON_WRENCH).c_str(), &instance().style_editor, ImGuiWindowFlags_NoFocusOnAppearing);
            callback();
            ImGui::End();
        }
    }
    static void color_themes_editor(std::function<void()> callback)
    {
        if (instance().color_themes_editor)
        {
            ImGui::Begin(Cool::icon_fmt("Color Themes: Editor", ICOMOON_WRENCH).c_str(), &instance().color_themes_editor, ImGuiWindowFlags_NoFocusOnAppearing);
            callback();
            ImGui::End();
        }
    }
    static void color_themes_advanced_config_window(std::function<void()> callback)
    {
        if (instance().color_themes_advanced_config_window)
        {
            ImGui::Begin(Cool::icon_fmt("Color Themes: Advanced Config", ICOMOON_WRENCH).c_str(), &instance().color_themes_advanced_config_window, ImGuiWindowFlags_NoFocusOnAppearing);
            callback();
            ImGui::End();
        }
    }
#if DEBUG
    [[nodiscard]] static auto imgui_item_picker() -> bool&
    {
        return instance().imgui_item_picker;
    }
#endif

private:
    struct Instance {
        bool test_message_console__window{false};
        bool log_when_autosaving{false};
        bool log_when_rendering_alpha_checkerboard_background{false};
        bool log_when_creating_textures{false};
        bool texture_library_debug_view{false};
        bool log_number_of_threads_in_the_thread_pool{false};
#if DEBUG
        bool log_opengl_info{false};
#endif
        bool test_presets__window{false};
        bool test_markdown_formatting_window{false};
        bool test_did_you_know{false};
        bool public_exhibition_mode{false};
        bool style_editor{false};
        bool color_themes_editor{false};
        bool color_themes_advanced_config_window{false};
#if DEBUG
        bool imgui_item_picker{false};
#endif

    private:
        // Serialization
        friend class cereal::access;
        template<class Archive>
        void serialize(Archive& archive)
        {
            archive(
#if DEBUG
                cereal::make_nvp("Test Message Console", test_message_console__window),
                cereal::make_nvp("Log when autosaving", log_when_autosaving),
                cereal::make_nvp("Log when rendering alpha checkerboard background", log_when_rendering_alpha_checkerboard_background),
                cereal::make_nvp("Log when creating textures", log_when_creating_textures),
                cereal::make_nvp("View Texture Library", texture_library_debug_view),
                cereal::make_nvp("Log the number of threads in the thread pool", log_number_of_threads_in_the_thread_pool),
                cereal::make_nvp("Log OpenGL info", log_opengl_info),
                cereal::make_nvp("Test Presets", test_presets__window),
                cereal::make_nvp("Test Markdown Formatting", test_markdown_formatting_window),
                cereal::make_nvp("Test \"Did you know?\"", test_did_you_know),
                cereal::make_nvp("Public exhibition mode", public_exhibition_mode),
                cereal::make_nvp("Style Editor", style_editor),
                cereal::make_nvp("Color Themes: Editor", color_themes_editor),
                cereal::make_nvp("Color Themes: Advanced Config", color_themes_advanced_config_window)
#else
                cereal::make_nvp("Test Message Console", test_message_console__window),
                cereal::make_nvp("Log when autosaving", log_when_autosaving),
                cereal::make_nvp("Log when rendering alpha checkerboard background", log_when_rendering_alpha_checkerboard_background),
                cereal::make_nvp("Log when creating textures", log_when_creating_textures),
                cereal::make_nvp("View Texture Library", texture_library_debug_view),
                cereal::make_nvp("Log the number of threads in the thread pool", log_number_of_threads_in_the_thread_pool),
                cereal::make_nvp("Test Presets", test_presets__window),
                cereal::make_nvp("Test Markdown Formatting", test_markdown_formatting_window),
                cereal::make_nvp("Test \"Did you know?\"", test_did_you_know),
                cereal::make_nvp("Public exhibition mode", public_exhibition_mode),
                cereal::make_nvp("Style Editor", style_editor),
                cereal::make_nvp("Color Themes: Editor", color_themes_editor),
                cereal::make_nvp("Color Themes: Advanced Config", color_themes_advanced_config_window)
#endif

            );
        }
    };

    static void reset_all()
    {
        instance().test_message_console__window                     = false;
        instance().log_when_autosaving                              = false;
        instance().log_when_rendering_alpha_checkerboard_background = false;
        instance().log_when_creating_textures                       = false;
        instance().texture_library_debug_view                       = false;
        instance().log_number_of_threads_in_the_thread_pool         = false;
#if DEBUG
        instance().log_opengl_info = false;
#endif
        instance().test_presets__window                = false;
        instance().test_markdown_formatting_window     = false;
        instance().test_did_you_know                   = false;
        instance().public_exhibition_mode              = false;
        instance().style_editor                        = false;
        instance().color_themes_editor                 = false;
        instance().color_themes_advanced_config_window = false;
    }

    static void save_to_file();
    static auto load_debug_options() -> Instance;

    static auto instance() -> Instance&
    {
        static auto the_instance = Instance{load_debug_options()};
        return the_instance;
    }

    template<typename... Ts>
    friend class Cool::DebugOptionsManager; // We go through this indirection so that only the files which include "DebugOptionsManager" can call `imgui_ui_for_all_options()`

    static void imgui_ui_for_all_options(std::string_view filter)
    {
        if (wafl::similarity_match({filter, "Test Message Console"}) >= wafl::Matches::Strongly)
        {
            Cool::ImGuiExtras::toggle("Test Message Console", &instance().test_message_console__window);
        }

        if (wafl::similarity_match({filter, "Log when autosaving"}) >= wafl::Matches::Strongly)
        {
            Cool::ImGuiExtras::toggle("Log when autosaving", &instance().log_when_autosaving);
        }

        if (wafl::similarity_match({filter, "Log when rendering alpha checkerboard background"}) >= wafl::Matches::Strongly)
        {
            Cool::ImGuiExtras::toggle("Log when rendering alpha checkerboard background", &instance().log_when_rendering_alpha_checkerboard_background);
        }

        if (wafl::similarity_match({filter, "Log when creating textures"}) >= wafl::Matches::Strongly)
        {
            Cool::ImGuiExtras::toggle("Log when creating textures", &instance().log_when_creating_textures);
        }

        if (wafl::similarity_match({filter, "View Texture Library"}) >= wafl::Matches::Strongly)
        {
            Cool::ImGuiExtras::toggle("View Texture Library", &instance().texture_library_debug_view);
        }

        if (wafl::similarity_match({filter, "Log the number of threads in the thread pool"}) >= wafl::Matches::Strongly)
        {
            Cool::ImGuiExtras::toggle("Log the number of threads in the thread pool", &instance().log_number_of_threads_in_the_thread_pool);
        }

#if DEBUG

        if (wafl::similarity_match({filter, "Log OpenGL info"}) >= wafl::Matches::Strongly)
        {
            Cool::ImGuiExtras::toggle("Log OpenGL info", &instance().log_opengl_info);
        }

#endif

        if (wafl::similarity_match({filter, "Test Presets"}) >= wafl::Matches::Strongly)
        {
            Cool::ImGuiExtras::toggle("Test Presets", &instance().test_presets__window);
        }

        if (wafl::similarity_match({filter, "Test Markdown Formatting"}) >= wafl::Matches::Strongly)
        {
            Cool::ImGuiExtras::toggle("Test Markdown Formatting", &instance().test_markdown_formatting_window);
        }

        if (wafl::similarity_match({filter, "Test \"Did you know?\""}) >= wafl::Matches::Strongly)
        {
            Cool::ImGuiExtras::toggle("Test \"Did you know?\"", &instance().test_did_you_know);
        }

        if (wafl::similarity_match({filter, "Public exhibition mode"}) >= wafl::Matches::Strongly)
        {
            Cool::ImGuiExtras::toggle("Public exhibition mode", &instance().public_exhibition_mode);
            Cool::ImGuiExtras::help_marker("Currently, simply resets the author info after sharing an image online.");
        }

        if (wafl::similarity_match({filter, "Style Editor"}) >= wafl::Matches::Strongly)
        {
            Cool::ImGuiExtras::toggle("Style Editor", &instance().style_editor);
        }

        if (wafl::similarity_match({filter, "Color Themes: Editor"}) >= wafl::Matches::Strongly)
        {
            Cool::ImGuiExtras::toggle("Color Themes: Editor", &instance().color_themes_editor);
        }

        if (wafl::similarity_match({filter, "Color Themes: Advanced Config"}) >= wafl::Matches::Strongly)
        {
            Cool::ImGuiExtras::toggle("Color Themes: Advanced Config", &instance().color_themes_advanced_config_window);
        }

#if DEBUG

        if (wafl::similarity_match({filter, "ImGui Item Picker"}) >= wafl::Matches::Strongly)
        {
            instance().imgui_item_picker = ImGui::Button("##ImGui Item Picker", {ImGui::GetFrameHeight(), ImGui::GetFrameHeight()});
            ImGui::SameLine(0.f, ImGui::GetStyle().ItemInnerSpacing.x);
            ImGui::Text("ImGui Item Picker");
            if (ImGui::IsItemClicked())
                instance().imgui_item_picker = true;

            Cool::ImGuiExtras::help_marker("Allows you to click on any ImGui widget and have your IDE break on it, allowing you to find the source code that generated it.\nShortcut: CTRL + SHIFT + I");
        }

#endif
    }

    static void toggle_first_option(std::string_view filter)
    {
        if (wafl::similarity_match({filter, "Test Message Console"}) >= wafl::Matches::Strongly)
        {
            instance().test_message_console__window = !instance().test_message_console__window;
            throw 0.f; // To understand why we need to throw, see `toggle_first_option()` in <Cool/DebugOptions/DebugOptionsManager.h>
        }

        if (wafl::similarity_match({filter, "Log when autosaving"}) >= wafl::Matches::Strongly)
        {
            instance().log_when_autosaving = !instance().log_when_autosaving;
            throw 0.f; // To understand why we need to throw, see `toggle_first_option()` in <Cool/DebugOptions/DebugOptionsManager.h>
        }

        if (wafl::similarity_match({filter, "Log when rendering alpha checkerboard background"}) >= wafl::Matches::Strongly)
        {
            instance().log_when_rendering_alpha_checkerboard_background = !instance().log_when_rendering_alpha_checkerboard_background;
            throw 0.f; // To understand why we need to throw, see `toggle_first_option()` in <Cool/DebugOptions/DebugOptionsManager.h>
        }

        if (wafl::similarity_match({filter, "Log when creating textures"}) >= wafl::Matches::Strongly)
        {
            instance().log_when_creating_textures = !instance().log_when_creating_textures;
            throw 0.f; // To understand why we need to throw, see `toggle_first_option()` in <Cool/DebugOptions/DebugOptionsManager.h>
        }

        if (wafl::similarity_match({filter, "View Texture Library"}) >= wafl::Matches::Strongly)
        {
            instance().texture_library_debug_view = !instance().texture_library_debug_view;
            throw 0.f; // To understand why we need to throw, see `toggle_first_option()` in <Cool/DebugOptions/DebugOptionsManager.h>
        }

        if (wafl::similarity_match({filter, "Log the number of threads in the thread pool"}) >= wafl::Matches::Strongly)
        {
            instance().log_number_of_threads_in_the_thread_pool = !instance().log_number_of_threads_in_the_thread_pool;
            throw 0.f; // To understand why we need to throw, see `toggle_first_option()` in <Cool/DebugOptions/DebugOptionsManager.h>
        }

#if DEBUG

        if (wafl::similarity_match({filter, "Log OpenGL info"}) >= wafl::Matches::Strongly)
        {
            instance().log_opengl_info = !instance().log_opengl_info;
            throw 0.f; // To understand why we need to throw, see `toggle_first_option()` in <Cool/DebugOptions/DebugOptionsManager.h>
        }

#endif

        if (wafl::similarity_match({filter, "Test Presets"}) >= wafl::Matches::Strongly)
        {
            instance().test_presets__window = !instance().test_presets__window;
            throw 0.f; // To understand why we need to throw, see `toggle_first_option()` in <Cool/DebugOptions/DebugOptionsManager.h>
        }

        if (wafl::similarity_match({filter, "Test Markdown Formatting"}) >= wafl::Matches::Strongly)
        {
            instance().test_markdown_formatting_window = !instance().test_markdown_formatting_window;
            throw 0.f; // To understand why we need to throw, see `toggle_first_option()` in <Cool/DebugOptions/DebugOptionsManager.h>
        }

        if (wafl::similarity_match({filter, "Test \"Did you know?\""}) >= wafl::Matches::Strongly)
        {
            instance().test_did_you_know = !instance().test_did_you_know;
            throw 0.f; // To understand why we need to throw, see `toggle_first_option()` in <Cool/DebugOptions/DebugOptionsManager.h>
        }

        if (wafl::similarity_match({filter, "Public exhibition mode"}) >= wafl::Matches::Strongly)
        {
            instance().public_exhibition_mode = !instance().public_exhibition_mode;
            throw 0.f; // To understand why we need to throw, see `toggle_first_option()` in <Cool/DebugOptions/DebugOptionsManager.h>
        }

        if (wafl::similarity_match({filter, "Style Editor"}) >= wafl::Matches::Strongly)
        {
            instance().style_editor = !instance().style_editor;
            throw 0.f; // To understand why we need to throw, see `toggle_first_option()` in <Cool/DebugOptions/DebugOptionsManager.h>
        }

        if (wafl::similarity_match({filter, "Color Themes: Editor"}) >= wafl::Matches::Strongly)
        {
            instance().color_themes_editor = !instance().color_themes_editor;
            throw 0.f; // To understand why we need to throw, see `toggle_first_option()` in <Cool/DebugOptions/DebugOptionsManager.h>
        }

        if (wafl::similarity_match({filter, "Color Themes: Advanced Config"}) >= wafl::Matches::Strongly)
        {
            instance().color_themes_advanced_config_window = !instance().color_themes_advanced_config_window;
            throw 0.f; // To understand why we need to throw, see `toggle_first_option()` in <Cool/DebugOptions/DebugOptionsManager.h>
        }

#if DEBUG

        if (wafl::similarity_match({filter, "ImGui Item Picker"}) >= wafl::Matches::Strongly)
        {
            instance().imgui_item_picker = !instance().imgui_item_picker;
            throw 0.f; // To understand why we need to throw, see `toggle_first_option()` in <Cool/DebugOptions/DebugOptionsManager.h>
        }

#endif
    }
};

} // namespace Cool
