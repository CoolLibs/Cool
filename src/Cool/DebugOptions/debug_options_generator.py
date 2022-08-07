# ---HOW TO---

# ------------

from dataclasses import dataclass


@dataclass
class DebugOption:
    name_in_code: str
    name_in_ui: str
    default_value: bool = False


def debug_options_variables(debug_options: list[DebugOption]):
    return "\n".join(map(lambda debug_option:
                         f"bool {debug_option.name_in_code}{{{'true' if debug_option.default_value else 'false'}}};",
                         debug_options))


def getters_for_debug_build(debug_options: list[DebugOption]):
    return "\n".join(map(lambda debug_option:
                         f"[[nodiscard]] static auto {debug_option.name_in_code}() -> bool& {{ return instance().{debug_option.name_in_code}; }}",
                         debug_options))


def getters_for_release_build(debug_options: list[DebugOption]):
    return "\n".join(map(lambda debug_option:
                         f"[[nodiscard]] static auto constexpr {debug_option.name_in_code}() -> bool {{ return false; }}",
                         debug_options))


def imgui_checkboxes_for_all_options(debug_options: list[DebugOption]):
    return "\n".join(map(lambda debug_option:
                         f'''
                        if (wafl::similarity_match({{filter, "{debug_option.name_in_ui}"}}) >= wafl::Matches::Strongly )
                            ImGui::Checkbox("{debug_option.name_in_ui}", &instance().{debug_option.name_in_code});
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
    // clang-format off
{getters_for_debug_build(debug_options)}
    // clang-format on

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
