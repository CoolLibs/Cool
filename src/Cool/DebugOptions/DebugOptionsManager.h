#pragma once
#if DEBUG

#include "DebugOptions.h"

namespace Cool {

// https://stackoverflow.com/questions/16387354/template-tuple-calling-a-function-on-each-element/37100197#comment84411734_37100197
#define COOL_DEBUG_OPTIONS_MANAGER_FOR_ALL_T(fn_name) std::apply([](auto&&... x) { (static_cast<void>(fn_name(std::forward<decltype(x)>(x))), ...); }, std::tuple<Ts...>{})

template<typename... Ts>
class DebugOptionsManager {
public:
    static void imgui_checkboxes_for_all_options()
    {
        ImGui::InputText("Filter", &instance().filter);
        if (ImGui::Button("Reset all debug options"))
        {
            reset_all();
        }
        ImGui::Separator();
        COOL_DEBUG_OPTIONS_MANAGER_FOR_ALL_T(imgui_checkboxes_for_all_options_impl);
    }

    struct AutoSerializer {
        ~AutoSerializer()
        {
            COOL_DEBUG_OPTIONS_MANAGER_FOR_ALL_T(save_to_file_impl);
        }
    };

    static void reset_all()
    {
        instance().filter = "";
        COOL_DEBUG_OPTIONS_MANAGER_FOR_ALL_T(reset_all_impl);
    }

private:
    struct Instance {
        std::string filter;
    };

    static auto instance() -> Instance&
    {
        {
            static auto the_instance = Instance{};
            return the_instance;
        }
    }

    template<typename T>
    static void imgui_checkboxes_for_all_options_impl(T&&)
    {
        T::imgui_checkboxes_for_all_options(instance().filter);
    }

    template<typename T>
    static void reset_all_impl(T&&)
    {
        T::reset_all();
    }

    template<typename T>
    static void save_to_file_impl(T&&)
    {
        T::save_to_file();
    }
};

} // namespace Cool

#endif