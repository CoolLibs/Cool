# ---HOW TO---

# ------------

from dataclasses import dataclass
from enum import Enum, auto


class Kind(Enum):
    BOOLEAN = auto()
    WINDOW = auto()


@dataclass
class DebugOption:
    name_in_code: str
    name_in_ui: str
    window_name: str = ""
    kind: Kind = Kind.BOOLEAN
    default_value: bool = False


def debug_options_variables(debug_options: list[DebugOption]):
    return "\n".join(map(lambda debug_option:
                         f"bool {debug_option.name_in_code}{{{'true' if debug_option.default_value else 'false'}}};",
                         debug_options))


def window_name(debug_option: DebugOption):
    return debug_option.window_name if debug_option.window_name else debug_option.name_in_ui


def option_implementation(debug_option: DebugOption):
    match debug_option.kind:
        case Kind.BOOLEAN:
            return f"[[nodiscard]] static auto {debug_option.name_in_code}() -> bool& {{ return instance().{debug_option.name_in_code}; }}"
        case _:
            return f"""static void {debug_option.name_in_code}(std::function<void()> callback)
            {{
                if (instance().{debug_option.name_in_code}) 
                {{
                    ImGui::Begin("{window_name(debug_option)}", &instance().{debug_option.name_in_code});
                    callback();
                    ImGui::End();
                }}
            }}"""


def options_implementations(debug_options: list[DebugOption]):
    return "\n".join(map(option_implementation, debug_options))


def passes_the_filter(debug_option: DebugOption):
    return f'wafl::similarity_match({{filter, "{debug_option.name_in_ui}"}}) >= wafl::Matches::Strongly'


def imgui_checkboxes_for_all_options(debug_options: list[DebugOption]):
    return "\n".join(map(lambda debug_option:
                         f'''
                        if ({passes_the_filter(debug_option)})
                            ImGui::Checkbox("{debug_option.name_in_ui}", &instance().{debug_option.name_in_code});
                        ''',
                         debug_options))


def toggle_first_checkbox(debug_options: list[DebugOption]):
    return "\n".join(map(lambda debug_option:
                         f'''
                        if ({passes_the_filter(debug_option)})
                        {{
                            instance().{debug_option.name_in_code} = !instance().{debug_option.name_in_code};
                            throw 0.f; // To understand why we need to throw, see `toggle_first_checkbox()` in <Cool/DebugOptions/DebugOptionsManager.h>
                        }}
                        ''',
                         debug_options))


def cereal_make_nvp(debug_options: list[DebugOption]):
    return ",\n".join(map(lambda debug_option:
                          f'cereal::make_nvp("{debug_option.name_in_ui}", {debug_option.name_in_code})',
                          debug_options))


def reset_all(debug_options: list[DebugOption]):
    return "\n".join(map(lambda debug_option:
                         f'instance().{debug_option.name_in_code} = {"true" if debug_option.default_value else "false"};',
                         debug_options))


def DebugOptions(debug_options: list[DebugOption], namespace: str, cache_file_name: str):
    return f"""
#if DEBUG

#include <Cool/Path/Path.h>
#include <Cool/Serialization/as_json.h>
#include <Cool/DebugOptions/DebugOptionsManager.h>
#include <wafl/wafl.hpp>

namespace {namespace} {{

class DebugOptions {{
public:
    {options_implementations(debug_options)}

private:
    struct Instance {{
        {debug_options_variables(debug_options)}

    private:
        // Serialization
        friend class cereal::access;
        template<class Archive>
        void serialize(Archive& archive)
        {{
            archive(
                {cereal_make_nvp(debug_options)}
            );
        }}
    }};

    static void reset_all()
    {{
        {reset_all(debug_options)}
    }}

    static void save_to_file()
    {{
        Cool::Serialization::to_json(
            instance(),
            Cool::Path::root() + "/{cache_file_name}.json",
            "Debug Options"
        );
    }}

    static auto load_debug_options() -> Instance
    {{
        auto the_instance = Instance{{}};
        Cool::Serialization::from_json(the_instance, Cool::Path::root() + "/{cache_file_name}.json");
        return the_instance;
    }}

    static auto instance() -> Instance&
    {{
        static auto the_instance = Instance{{load_debug_options()}};
        return the_instance;
    }}

    template<typename... Ts>
    friend class Cool::DebugOptionsManager; // We go through this indirection so that only the files which include "DebugOptionsManager" can call `imgui_checkboxes_for_all_options()`
    
    static void imgui_checkboxes_for_all_options(std::string_view filter)
    {{
        {imgui_checkboxes_for_all_options(debug_options)}
    }}

    static void toggle_first_checkbox(std::string_view filter)
    {{
        {toggle_first_checkbox(debug_options)}
    }}
}};

}} // namespace {namespace}

#endif
"""


def generate_debug_options(
    output_folder: str,
    namespace: str,
    cache_file_name: str,
    debug_options: list[DebugOption],
):
    import os
    from pathlib import Path
    from importlib.machinery import SourceFileLoader
    generate_files = SourceFileLoader(
        "generate_files",
        os.path.join(Path(os.path.abspath(__file__)
                          ).parent.parent.parent.parent, "tooling", "generate_files.py")).load_module()

    def fn():
        return DebugOptions(debug_options=debug_options, namespace=namespace, cache_file_name=cache_file_name)
    fn.__name__ = "DebugOptions"
    generate_files.generate(
        folder=output_folder,
        files=[
            fn,
        ],
    )
