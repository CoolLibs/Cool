/* -----------------------------------------------------------------------------
 * This file was automatically generated by a Python script.
 * PLEASE DON'T EDIT IT DIRECTLY, your changes would be overwritten the next time the script is run.
 * Instead, go to "generate_debug_options.py" and edit the "DebugOptions" function there.
 * -----------------------------------------------------------------------------
 */

#if DEBUG

#include <Cool/DebugOptions/DebugOptionsManager.h>
#include <Cool/ImGui/ImGuiExtras.h>
#include <Cool/Path/Path.h>
#include <Cool/Serialization/as_json.h>
#include <wafl/wafl.hpp>

namespace Cool {

class DebugOptions {
public:
    static void test_message_console__window(std::function<void()> callback)
    {
        if (instance().test_message_console__window)
        {
            ImGui::Begin("Test Message Console", &instance().test_message_console__window);
            callback();
            ImGui::End();
        }
    }
    [[nodiscard]] static auto log_when_creating_icon() -> bool& { return instance().log_when_creating_icon; }
    [[nodiscard]] static auto log_number_of_threads_in_the_thread_pool() -> bool& { return instance().log_number_of_threads_in_the_thread_pool; }
    [[nodiscard]] static auto log_opengl_info() -> bool& { return instance().log_opengl_info; }
    static void               test_presets__window(std::function<void()> callback)
    {
        if (instance().test_presets__window)
        {
            ImGui::Begin("Test Presets", &instance().test_presets__window);
            callback();
            ImGui::End();
        }
    }
    [[nodiscard]] static auto imgui_item_picker() -> bool& { return instance().imgui_item_picker; }

private:
    struct Instance {
        bool test_message_console__window{false};
        bool log_when_creating_icon{false};
        bool log_number_of_threads_in_the_thread_pool{false};
        bool log_opengl_info{false};
        bool test_presets__window{false};
        bool imgui_item_picker{false};

    private:
        // Serialization
        friend class cereal::access;
        template<class Archive>
        void serialize(Archive& archive)
        {
            archive(
                cereal::make_nvp("Test Message Console", test_message_console__window),
                cereal::make_nvp("Log when creating icon", log_when_creating_icon),
                cereal::make_nvp("Log the number of threads in the thread pool", log_number_of_threads_in_the_thread_pool),
                cereal::make_nvp("Log OpenGL info", log_opengl_info),
                cereal::make_nvp("Test Presets", test_presets__window)
            );
        }
    };

    static void reset_all()
    {
        instance().test_message_console__window             = false;
        instance().log_when_creating_icon                   = false;
        instance().log_number_of_threads_in_the_thread_pool = false;
        instance().log_opengl_info                          = false;
        instance().test_presets__window                     = false;
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
                    .category         = "Loading Debug Options",
                    .detailed_message = message,
                    .severity         = Cool::MessageSeverity::Warning,
                };
            },
                               Cool::Log::Debug::console());
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
        if (wafl::similarity_match({filter, "Test Message Console"}) >= wafl::Matches::Strongly)
        {
            ImGui::Checkbox("Test Message Console", &instance().test_message_console__window);
        }

        if (wafl::similarity_match({filter, "Log when creating icon"}) >= wafl::Matches::Strongly)
        {
            ImGui::Checkbox("Log when creating icon", &instance().log_when_creating_icon);
        }

        if (wafl::similarity_match({filter, "Log the number of threads in the thread pool"}) >= wafl::Matches::Strongly)
        {
            ImGui::Checkbox("Log the number of threads in the thread pool", &instance().log_number_of_threads_in_the_thread_pool);
        }

        if (wafl::similarity_match({filter, "Log OpenGL info"}) >= wafl::Matches::Strongly)
        {
            ImGui::Checkbox("Log OpenGL info", &instance().log_opengl_info);
        }

        if (wafl::similarity_match({filter, "Test Presets"}) >= wafl::Matches::Strongly)
        {
            ImGui::Checkbox("Test Presets", &instance().test_presets__window);
        }

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
    }

    static void toggle_first_option(std::string_view filter)
    {
        if (wafl::similarity_match({filter, "Test Message Console"}) >= wafl::Matches::Strongly)
        {
            instance().test_message_console__window = !instance().test_message_console__window;
            throw 0.f; // To understand why we need to throw, see `toggle_first_option()` in <Cool/DebugOptions/DebugOptionsManager.h>
        }

        if (wafl::similarity_match({filter, "Log when creating icon"}) >= wafl::Matches::Strongly)
        {
            instance().log_when_creating_icon = !instance().log_when_creating_icon;
            throw 0.f; // To understand why we need to throw, see `toggle_first_option()` in <Cool/DebugOptions/DebugOptionsManager.h>
        }

        if (wafl::similarity_match({filter, "Log the number of threads in the thread pool"}) >= wafl::Matches::Strongly)
        {
            instance().log_number_of_threads_in_the_thread_pool = !instance().log_number_of_threads_in_the_thread_pool;
            throw 0.f; // To understand why we need to throw, see `toggle_first_option()` in <Cool/DebugOptions/DebugOptionsManager.h>
        }

        if (wafl::similarity_match({filter, "Log OpenGL info"}) >= wafl::Matches::Strongly)
        {
            instance().log_opengl_info = !instance().log_opengl_info;
            throw 0.f; // To understand why we need to throw, see `toggle_first_option()` in <Cool/DebugOptions/DebugOptionsManager.h>
        }

        if (wafl::similarity_match({filter, "Test Presets"}) >= wafl::Matches::Strongly)
        {
            instance().test_presets__window = !instance().test_presets__window;
            throw 0.f; // To understand why we need to throw, see `toggle_first_option()` in <Cool/DebugOptions/DebugOptionsManager.h>
        }

        if (wafl::similarity_match({filter, "ImGui Item Picker"}) >= wafl::Matches::Strongly)
        {
            instance().imgui_item_picker = !instance().imgui_item_picker;
            throw 0.f; // To understand why we need to throw, see `toggle_first_option()` in <Cool/DebugOptions/DebugOptionsManager.h>
        }
    }
};

} // namespace Cool

#endif
