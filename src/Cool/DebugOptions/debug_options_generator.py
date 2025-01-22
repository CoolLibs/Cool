# ---HOW TO---

# ------------

from dataclasses import dataclass
from enum import Enum, auto


class Kind(Enum):
    CHECKBOX = auto()
    WINDOW = auto()
    BUTTON = auto()


@dataclass
class DebugOption:
    name_in_code: str
    name_in_ui: str
    available_in_release: bool
    window_name: str = ""
    kind: Kind = Kind.CHECKBOX
    default_value_in_debug: bool = False
    default_value_in_release: bool = False
    detailed_description: str = ""


def build_type(code: str, debug_option: DebugOption) -> str:
    if debug_option.available_in_release:
        return code
    else:
        return f"""#if DEBUG
{code}
#endif"""


def default_value(debug_option: DebugOption):
    if (
        not debug_option.available_in_release
        or debug_option.default_value_in_release == debug_option.default_value_in_debug
    ):
        return "true" if debug_option.default_value_in_debug else "false"
    return f"""
#if defined(DEBUG)
{"true" if debug_option.default_value_in_debug else "false"}
#else
{"true" if debug_option.default_value_in_release else "false"}
#endif
"""


def window_name(debug_option: DebugOption):
    return (
        debug_option.window_name
        if debug_option.window_name
        else debug_option.name_in_ui
    )


def option_implementation(debug_option: DebugOption):
    def code():
        match debug_option.kind:
            case Kind.CHECKBOX | Kind.BUTTON:
                return f'[[nodiscard]] static auto {debug_option.name_in_code}() -> bool& {{ return json().get<bool>("{debug_option.name_in_ui}", {default_value(debug_option)}); }}'
            case Kind.WINDOW:
                return f"""static void {debug_option.name_in_code}(std::function<void()> callback)
                {{
                    bool& val = json().get<bool>("{debug_option.name_in_ui}", {default_value(debug_option)});
                    if (val) 
                    {{
                        ImGui::Begin(Cool::icon_fmt("{window_name(debug_option)}", ICOMOON_WRENCH).c_str(), &val, ImGuiWindowFlags_NoFocusOnAppearing);
                        callback();
                        ImGui::End();
                        if (!val) // Window has just been closed manually by the user
                            json().save();
                    }}
                }}"""
            case _:
                return ""

    return build_type(code(), debug_option)


def imgui_widget(debug_option: DebugOption):
    match debug_option.kind:
        case Kind.CHECKBOX | Kind.WINDOW:
            return f"""
            bool& val = json().get<bool>("{debug_option.name_in_ui}", {default_value(debug_option)});
            if(Cool::ImGuiExtras::toggle("{debug_option.name_in_ui}", &val))
                json().save();
            """
        case Kind.BUTTON:
            return f"""
            json().get<bool>("{debug_option.name_in_ui}", false) = ImGui::Button("##{debug_option.name_in_ui}", {{ImGui::GetFrameHeight(), ImGui::GetFrameHeight()}});
            ImGui::SameLine(0.f, ImGui::GetStyle().ItemInnerSpacing.x);
            ImGui::Text("{debug_option.name_in_ui}");
            if (ImGui::IsItemClicked())
                json().get<bool>("{debug_option.name_in_ui}", true) = true;
            """


def options_implementations(debug_options: list[DebugOption]):
    return "\n".join(map(option_implementation, debug_options))


def passes_the_filter(debug_option: DebugOption):
    return f'wafl::similarity_match({{filter, "{debug_option.name_in_ui}"}}) >= wafl::Matches::Strongly'


def imgui_ui_for_all_options(debug_options: list[DebugOption]):
    return "\n".join(
        map(
            lambda debug_option: build_type(
                f"""
                        if ({passes_the_filter(debug_option)})
                        {{
                            {imgui_widget(debug_option)}
                            {f'Cool::ImGuiExtras::help_marker("{debug_option.detailed_description}");' if debug_option.detailed_description else ""}
                        }}
                        """,
                debug_option,
            ),
            debug_options,
        )
    )


def toggle_first_option(debug_options: list[DebugOption]):
    return "\n".join(
        map(
            lambda debug_option: build_type(
                f"""
                        if ({passes_the_filter(debug_option)})
                        {{
                            bool& val = json().get<bool>("{debug_option.name_in_ui}", {default_value(debug_option)});
                            val = !val;
                            throw 0.f; // To understand why we need to throw, see `toggle_first_option()` in <Cool/DebugOptions/DebugOptionsManager.h>
                        }}
                        """,
                debug_option,
            ),
            debug_options,
        )
    )


def filter_out_buttons(debug_options: list[DebugOption]):
    return filter(lambda dbg: dbg.kind != Kind.BUTTON, debug_options)


def reset_all(debug_options: list[DebugOption]):
    return "\n".join(
        map(
            lambda debug_option: build_type(
                f'json().get<bool>("{debug_option.name_in_ui}", false) = {default_value(debug_option)};',
                debug_option,
            ),
            filter_out_buttons(debug_options),
        )
    )


def DebugOptions(
    debug_options: list[DebugOption],
    namespace: str,
    cache_file_name: str,
    always_show_warnings: bool,
):
    backslash = "\\"
    return f"""
#include "Cool/ImGui/ImGuiExtras.h"
#include "wafl/wafl.hpp"
#include "Cool/Serialization/JsonAutoSerializer.hpp"
#include "Cool/Path/Path.h"

namespace Cool {{
    template<typename... Ts>
    class DebugOptionsManager; // Forward declare this class so that the friend declaration that comes later on doesn't fail.
}}

namespace {namespace} {{

class DebugOptions {{
public:
    {options_implementations(debug_options)}

private:
    static auto json() -> Cool::JsonAutoSerializer<bool>&
    {{
        static auto the_json = Cool::JsonAutoSerializer<bool>{{Cool::Path::user_data() / "{cache_file_name}.json", {"true" if always_show_warnings else "false"}}};
        return the_json;
    }}

    static void reset_all()
    {{
        {reset_all(debug_options)}
        json().save();
    }}

    static void update()
    {{
        json().update();
    }}

    template<typename... Ts>
    friend class Cool::DebugOptionsManager; // We go through this indirection so that only the files which include "DebugOptionsManager" can call `imgui_ui_for_all_options()`
    
    static void imgui_ui_for_all_options(std::string_view filter)
    {{
        {imgui_ui_for_all_options(debug_options)}
    }}

    static void toggle_first_option(std::string_view filter)
    {{
        {toggle_first_option(debug_options)}
    }}
}};

}} // namespace {namespace}
"""


def generate_debug_options(
    output_folder: str,
    namespace: str,
    cache_file_name: str,
    debug_options: list[DebugOption],
    always_show_warnings: bool = False,
):
    import os
    from pathlib import Path
    from importlib.machinery import SourceFileLoader

    generate_files = SourceFileLoader(
        "generate_files",
        os.path.join(
            Path(os.path.abspath(__file__)).parent.parent.parent.parent,
            "tooling",
            "generate_files.py",
        ),
    ).load_module()

    def fn():
        return DebugOptions(
            debug_options=debug_options,
            namespace=namespace,
            cache_file_name=cache_file_name,
            always_show_warnings=always_show_warnings,
        )

    fn.__name__ = "DebugOptions"

    generate_files.generate(
        folder=output_folder,
        files=[
            fn,
        ],
    )
