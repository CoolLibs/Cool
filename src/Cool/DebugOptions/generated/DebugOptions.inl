/* -----------------------------------------------------------------------------
 * This file was automatically generated by a Python script.
 * PLEASE DON'T EDIT IT DIRECTLY, your changes would be overwritten the next time the script is run.
 * Instead, go to "generate_debug_options.py" and edit the "DebugOptions" function there.
 * -----------------------------------------------------------------------------
 */

#include <Cool/ImGui/ImGuiExtras.h>
#include <Cool/Path/Path.h>
#include <Cool/Serialization/as_json.h>
#include <wafl/wafl.hpp>

namespace Cool {
template<typename... Ts>
class DebugOptionsManager; // Forward declare this class so that the friend declaration that comes later on doesn't fail.
}

namespace Cool {

class DebugOptions {
public:
#if DEBUG
    static void test_message_console__window(std::function<void()> callback)
    {
        if (instance().test_message_console__window)
        {
            ImGui::Begin("Test Message Console", &instance().test_message_console__window);
            callback();
            ImGui::End();
        }
    }
#endif
    [[nodiscard]] static auto log_when_autosaving() -> bool&
    {
        return instance().log_when_autosaving;
    }
    [[nodiscard]] static auto log_when_rendering_alpha_checkerboard_background() -> bool& { return instance().log_when_rendering_alpha_checkerboard_background; }
    [[nodiscard]] static auto log_when_creating_textures() -> bool& { return instance().log_when_creating_textures; }
    static void               texture_library_debug_view(std::function<void()> callback)
    {
        if (instance().texture_library_debug_view)
        {
            ImGui::Begin("Texture Library", &instance().texture_library_debug_view);
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
#if DEBUG
    static void test_presets__window(std::function<void()> callback)
    {
        if (instance().test_presets__window)
        {
            ImGui::Begin("Test Presets", &instance().test_presets__window);
            callback();
            ImGui::End();
        }
    }
#endif
    static void color_themes_advanced_config_window(std::function<void()> callback)
    {
        if (instance().color_themes_advanced_config_window)
        {
            ImGui::Begin("Color Themes: Advanced Config", &instance().color_themes_advanced_config_window);
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
#if DEBUG
        bool test_message_console__window{false};
#endif
        bool log_when_autosaving{false};
        bool log_when_rendering_alpha_checkerboard_background{false};
        bool log_when_creating_textures{false};
        bool texture_library_debug_view{false};
        bool log_number_of_threads_in_the_thread_pool{false};
#if DEBUG
        bool log_opengl_info{false};
#endif
#if DEBUG
        bool test_presets__window{false};
#endif
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
                cereal::make_nvp("Color Themes: Advanced Config", color_themes_advanced_config_window)
#else
                cereal::make_nvp("Log when autosaving", log_when_autosaving),
                cereal::make_nvp("Log when rendering alpha checkerboard background", log_when_rendering_alpha_checkerboard_background),
                cereal::make_nvp("Log when creating textures", log_when_creating_textures),
                cereal::make_nvp("View Texture Library", texture_library_debug_view),
                cereal::make_nvp("Log the number of threads in the thread pool", log_number_of_threads_in_the_thread_pool),
                cereal::make_nvp("Color Themes: Advanced Config", color_themes_advanced_config_window)
#endif

            );
        }
    };

    static void reset_all()
    {
#if DEBUG
        instance().test_message_console__window = false;
#endif
        instance().log_when_autosaving                              = false;
        instance().log_when_rendering_alpha_checkerboard_background = false;
        instance().log_when_creating_textures                       = false;
        instance().texture_library_debug_view                       = false;
        instance().log_number_of_threads_in_the_thread_pool         = false;
#if DEBUG
        instance().log_opengl_info = false;
#endif
#if DEBUG
        instance().test_presets__window = false;
#endif
        instance().color_themes_advanced_config_window = false;
    }

    static void save_to_file()
    {
        Cool::Serialization::to_json(
            instance(),
            Cool::Path::root() / "cache/debug-options-cool.json",
            "Debug Options"
        );
    }

    static auto load_debug_options() -> Instance
    {
        auto the_instance = Instance{};
        Cool::Serialization::from_json(the_instance, Cool::Path::root() / "cache/debug-options-cool.json")
            .send_error_if_any([](const std::string& message) {
                return Cool::Message{
                    .category = "Loading Debug Options",
                    .message  = message,
                    .severity = Cool::MessageSeverity::Warning,
                };
            },
                               Cool::Log::ToUser::console());
        return the_instance;
    }

    static auto instance() -> Instance&
    {
        static auto the_instance = Instance{load_debug_options()};
        return the_instance;
    }

    template<typename... Ts>
    friend class Cool::DebugOptionsManager; // We go through this indirection so that only the files which include "DebugOptionsManager" can call `imgui_ui_for_all_options()`

    static void imgui_ui_for_all_options(std::string_view filter)
    {
#if DEBUG

        if (wafl::similarity_match({filter, "Test Message Console"}) >= wafl::Matches::Strongly)
        {
            ImGui::Checkbox("Test Message Console", &instance().test_message_console__window);
        }

#endif

        if (wafl::similarity_match({filter, "Log when autosaving"}) >= wafl::Matches::Strongly)
        {
            ImGui::Checkbox("Log when autosaving", &instance().log_when_autosaving);
        }

        if (wafl::similarity_match({filter, "Log when rendering alpha checkerboard background"}) >= wafl::Matches::Strongly)
        {
            ImGui::Checkbox("Log when rendering alpha checkerboard background", &instance().log_when_rendering_alpha_checkerboard_background);
        }

        if (wafl::similarity_match({filter, "Log when creating textures"}) >= wafl::Matches::Strongly)
        {
            ImGui::Checkbox("Log when creating textures", &instance().log_when_creating_textures);
        }

        if (wafl::similarity_match({filter, "View Texture Library"}) >= wafl::Matches::Strongly)
        {
            ImGui::Checkbox("View Texture Library", &instance().texture_library_debug_view);
        }

        if (wafl::similarity_match({filter, "Log the number of threads in the thread pool"}) >= wafl::Matches::Strongly)
        {
            ImGui::Checkbox("Log the number of threads in the thread pool", &instance().log_number_of_threads_in_the_thread_pool);
        }

#if DEBUG

        if (wafl::similarity_match({filter, "Log OpenGL info"}) >= wafl::Matches::Strongly)
        {
            ImGui::Checkbox("Log OpenGL info", &instance().log_opengl_info);
        }

#endif
#if DEBUG

        if (wafl::similarity_match({filter, "Test Presets"}) >= wafl::Matches::Strongly)
        {
            ImGui::Checkbox("Test Presets", &instance().test_presets__window);
        }

#endif

        if (wafl::similarity_match({filter, "Color Themes: Advanced Config"}) >= wafl::Matches::Strongly)
        {
            ImGui::Checkbox("Color Themes: Advanced Config", &instance().color_themes_advanced_config_window);
        }

#if DEBUG

        if (wafl::similarity_match({filter, "ImGui Item Picker"}) >= wafl::Matches::Strongly)
        {
            instance().imgui_item_picker = ImGui::Button("##ImGui Item Picker", {ImGui::GetFrameHeight(), ImGui::GetFrameHeight()});
            ImGui::SameLine(0.f, ImGui::GetStyle().ItemInnerSpacing.x);
            ImGui::Text("ImGui Item Picker");
            if (ImGui::IsItemClicked())
                instance().imgui_item_picker = true;

            ImGui::SameLine();
            Cool::ImGuiExtras::help_marker("Allows you to click on any ImGui widget and have your IDE break on it, allowing you to find the source code that generated it.");
        }

#endif
    }

    static void toggle_first_option(std::string_view filter)
    {
#if DEBUG

        if (wafl::similarity_match({filter, "Test Message Console"}) >= wafl::Matches::Strongly)
        {
            instance().test_message_console__window = !instance().test_message_console__window;
            throw 0.f; // To understand why we need to throw, see `toggle_first_option()` in <Cool/DebugOptions/DebugOptionsManager.h>
        }

#endif

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
#if DEBUG

        if (wafl::similarity_match({filter, "Test Presets"}) >= wafl::Matches::Strongly)
        {
            instance().test_presets__window = !instance().test_presets__window;
            throw 0.f; // To understand why we need to throw, see `toggle_first_option()` in <Cool/DebugOptions/DebugOptionsManager.h>
        }

#endif

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
