# ---HOW TO---

# ------------

from dataclasses import dataclass


@dataclass
class DebugOption:
    name_in_code: str
    name_in_ui: str
    default_value: bool = False


def all_debug_options():
    return [
        DebugOption(
            name_in_code="show_framerate_window",
            name_in_ui="Framerate window",
            default_value=True,
        ),
        DebugOption(
            name_in_code="show_imgui_demo_window",
            name_in_ui="ImGui Demo window",
        ),
        DebugOption(
            name_in_code="show_commands_and_registries_debug_windows",
            name_in_ui="Commands and Registries windows",
        ),
        DebugOption(
            name_in_code="log_when_rendering",
            name_in_ui="Log when rendering",
        ),
        DebugOption(
            name_in_code="test_all_variable_widgets",
            name_in_ui="Test all Variable Widgets",
        ),
        DebugOption(
            name_in_code="test_message_console",
            name_in_ui="Test Message Console",
        ),
    ]


def debug_options_variables():
    return "\n".join(map(lambda debug_option:
                         f"bool {debug_option.name_in_code}{{{'true' if debug_option.default_value else 'false'}}};",
                         all_debug_options()))


def getters_for_debug_build():
    return "\n".join(map(lambda debug_option:
                         f"[[nodiscard]] static auto {debug_option.name_in_code}() -> bool& {{ return instance().{debug_option.name_in_code}; }}",
                         all_debug_options()))


def getters_for_release_build():
    return "\n".join(map(lambda debug_option:
                         f"[[nodiscard]] static auto constexpr {debug_option.name_in_code}() -> bool {{ return false; }}",
                         all_debug_options()))


def imgui_checkboxes_for_all_options():
    return "\n".join(map(lambda debug_option:
                         f'if (instance().filter.PassFilter("{debug_option.name_in_ui}")) ImGui::Checkbox("{debug_option.name_in_ui}", &instance().{debug_option.name_in_code});',
                         all_debug_options()))


def cereal_make_nvp():
    return ",\n".join(map(lambda debug_option:
                          f'cereal::make_nvp("{debug_option.name_in_ui}", {debug_option.name_in_code})',
                          all_debug_options()))


def reset_all():
    return "\n".join(map(lambda debug_option:
                         f'instance().{debug_option.name_in_code} = {"true" if debug_option.default_value else "false"};',
                         all_debug_options()))


def DebugOptions():
    return f"""
#pragma once

#include <Cool/Path/Path.h>
#include <Cool/Serialization/as_json.h>

namespace Lab {{

class DebugOptions {{
public:
#if DEBUG
// clang-format off
{getters_for_debug_build()}
#else
{getters_for_release_build()}
#endif
// clang-format on

private:
#if DEBUG
    struct Instance {{
        {debug_options_variables()}
        ImGuiTextFilter filter;

    private:
        // Serialization
        friend class cereal::access;
        template<class Archive>
        void serialize(Archive& archive)
        {{
            archive(
                {cereal_make_nvp()}
            );
        }}
    }};

    static void reset_all()
    {{
        instance().filter.Clear();
        {reset_all()}
    }}

    static auto load_debug_options() -> Instance
    {{
        auto the_instance = Instance{{}};
        Cool::Serialization::from_json(the_instance, Cool::Path::root() + "/cache--debug-options.json");
        return the_instance;
    }}

    static auto instance() -> Instance&
    {{
        static auto the_instance = Instance{{load_debug_options()}};
        return the_instance;
    }}
#endif

    friend class DebugOptionsDetails; // We go through this indirection so that only the files which include "DebugOptionsDetails" can call `imgui_checkboxes_for_all_options()`
    static void imgui_checkboxes_for_all_options()
    {{
#if DEBUG
        instance().filter.Draw("Filter");
        if (ImGui::Button("Reset all debug options"))
        {{
            reset_all();
        }}
        ImGui::Separator();
        {imgui_checkboxes_for_all_options()}
#endif
    }}
}};

}} // namespace Lab
"""


def generate_debug_options(
    output_folder: str,
    debug_options: list[DebugOption]
):
    # HACK: Python doesn't allow us to import from a parent folder (e.g. tooling.generate_files)
    # So we need to add the path manually to sys.path
    import os
    import sys
    from pathlib import Path
    sys.path.append(os.path.join(
        Path(os.path.abspath(__file__)).parent.parent.parent.parent,
        "tooling")
    )
    import generate_files
    # End of HACK

    generate_files.generate(
        folder=output_folder,
        files=[
            DebugOptions,
        ],
    )


if __name__ == '__main__':
    generate_debug_options(
        output_folder="generated",
        debug_options=all_debug_options(),
    )
