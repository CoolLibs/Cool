# ---HOW TO---
# Create a function with the same name as the file you want to generate
# that returns a string with the content you want to generate.
#
# Then add your function to the call to `generate()` at the end of the file.
#
# You can use `all_variable_types()` to get all the variable types we use in CoolLab.
# ------------

from dataclasses import dataclass
from typing import List


@dataclass
class VariableMetadata:
    name_in_shader: str
    field_name: str
    pretty_name: str
    type: str
    default_value: str


@dataclass
class VariableType:
    type: str
    metadatas: list[VariableMetadata]
    do_generate_get_default_metadata: bool = True


def all_variable_types_and_metadatas():
    return [
        VariableType(
            type="bool",
            metadatas=[]
        ),
        VariableType(
            type="int",
            metadatas=[
                VariableMetadata(
                    name_in_shader="min",
                    field_name="min_value",
                    pretty_name="Min Value",
                    type="int",
                    default_value="0",
                ),
                VariableMetadata(
                    name_in_shader="max",
                    field_name="max_value",
                    pretty_name="Max Value",
                    type="int",
                    default_value="10",
                ),
            ]
        ),
        VariableType(
            type="float",
            metadatas=[
                VariableMetadata(
                    name_in_shader="min",
                    field_name="min_value",
                    pretty_name="Min Value",
                    type="float",
                    default_value="0.f",
                ),
                VariableMetadata(
                    name_in_shader="max",
                    field_name="max_value",
                    pretty_name="Max Value",
                    type="float",
                    default_value="1.f",
                ),
            ],
        ),
        VariableType(
            type="glm::vec2",
            metadatas=[],
        ),
        VariableType(
            type="glm::vec3",
            metadatas=[],
        ),
        VariableType(
            type="glm::vec4",
            metadatas=[],
        ),
        VariableType(
            type="Cool::RgbColor",
            metadatas=[
                VariableMetadata(
                    name_in_shader="hdr",
                    field_name="is_hdr",
                    pretty_name="Is HDR",
                    type="bool",
                    default_value="true",
                ),
            ],
            do_generate_get_default_metadata=False,
        ),
        VariableType(
            type="Cool::Camera",
            metadatas=[],
        ),
        VariableType(
            type="Cool::Angle",
            metadatas=[],
        ),
        VariableType(
            type="Cool::Direction2D",
            metadatas=[],
        ),
        VariableType(
            type="Cool::Hue",
            metadatas=[],
        ),
        # VariableType(
        #     type="Cool::ColorPalette",
        #     metadatas=[],
        # ),
    ]


def all_variable_types():
    return map(lambda type_and_meta: type_and_meta.type,
               all_variable_types_and_metadatas())


def strip_namespace(variable_type):
    return variable_type[variable_type.rfind(":")+1:]


def all_variable_types_without_namespaces():
    return map(lambda variable_type: strip_namespace(variable_type),
               all_variable_types())


def all_variable_types_and_metadatas_without_namespaces():
    def strip_namespace_on_variable_type(type_and_meta):
        return VariableType(
            type=strip_namespace(type_and_meta.type),
            metadatas=type_and_meta.metadatas
        )
    return map(lambda variable_type_and_metadatas: strip_namespace_on_variable_type(variable_type_and_metadatas),
               all_variable_types_and_metadatas())


def all_variable_includes():
    return f"""
#include <Cool/Camera/Camera.h>
#include <Cool/Variables/Variables.h>
"""


def register_set_variable_commands():
    commands = ""
    reversible_commands = ""
    for variable_type in all_variable_types():
        commands += f"LAB_REGISTER_COMMAND(Lab::Command_SetVariable<{variable_type}>)\n"
        reversible_commands += f"LAB_REGISTER_REVERSIBLE_COMMAND(Lab::ReversibleCommand_SetVariable<{variable_type}>)\n"
    return f"""
{commands}
{reversible_commands}
"""


def register_set_variable_metadata_commands():
    out = "\n"
    for variable_type in all_variable_types():
        out += f"LAB_REGISTER_COMMAND(Lab::Command_SetVariableMetadata<{variable_type}>)\n"
    return out


def VariableRegistries():
    return "\n" + "using VariableRegistries = reg::Registries<\n" + ",\n".join(
        map(lambda var_type: f"    Cool::Variable<{var_type}>", all_variable_types())) + "\n>;"


def AnyInput():
    return "\n" + "using AnyInput = std::variant<\n" + ",\n".join(
        map(lambda var_type: f"    Input<{var_type}>", all_variable_types())) + "\n>;"


def AnyInputRef():
    return "\n" + "using AnyInputRef = std::variant<\n" + ",\n".join(
        map(lambda var_type: f"    std::reference_wrapper<Input<{var_type}>>", all_variable_types())) + "\n>;"


def AnyInputRefToConst():
    return "\n" + "using AnyInputRefToConst = std::variant<\n" + ",\n".join(
        map(lambda var_type: f"    std::reference_wrapper<const Input<{var_type}>>", all_variable_types())) + "\n>;"


def find_metadatas_in_string():
    out = ""
    for variable_types_and_metadatas in all_variable_types_and_metadatas():
        if variable_types_and_metadatas.do_generate_get_default_metadata == True:
            out += f'''
template<>
auto get_default_metadata(std::string_view{" key_values" if variable_types_and_metadatas.metadatas else ""}) -> Cool::VariableMetadata<{variable_types_and_metadatas.type}>
{{
    Cool::VariableMetadata<{variable_types_and_metadatas.type}> metadata{{}};
'''
            for variable_metadatas in variable_types_and_metadatas.metadatas:
                out += f'''
    const auto {variable_metadatas.field_name} = Cool::String::find_value_for_given_key<{variable_metadatas.type}>(key_values, "{variable_metadatas.name_in_shader}");
    if ({variable_metadatas.field_name})
    {{
        metadata.{variable_metadatas.field_name} = *{variable_metadatas.field_name};
    }}
'''
            out += "    return metadata;\n}\n"
    return out


def variables_includes():
    out = "\n"
    for variable_type in all_variable_types_without_namespaces():
        out += f'''#include <Cool/Variables/Variable_{variable_type}.h>
'''
    return out


def cereal_make_nvp(metadatas: List[VariableMetadata]):
    return ",\n".join(map(lambda meta:
                          f'           cereal::make_nvp("{meta.pretty_name}", {meta.field_name})', metadatas)
                      )


def cereal_serialize_body(metadatas: List[VariableMetadata]):
    return f'''
        archive(
{cereal_make_nvp(metadatas)}
        );'''


def metadatas_definitions(metadatas: List[VariableMetadata]):
    return "\n    ".join(map(
        lambda metadata:
            f"{metadata.type} {metadata.field_name}{{{metadata.default_value}}};",
            metadatas))


def variable_definition_factory(variable_type_and_metadatas):
    def variable_definition():
        has_metadatas = len(variable_type_and_metadatas.metadatas) > 0
        out = "\n"
        out += f'''
template<>
struct VariableMetadata<{variable_type_and_metadatas.type}> {{
    {metadatas_definitions(variable_type_and_metadatas.metadatas)}

    friend auto operator<=>(const VariableMetadata<{variable_type_and_metadatas.type}>&, const VariableMetadata<{variable_type_and_metadatas.type}>&) = default;

private:
    // Serialisation
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive&{" archive" if has_metadatas else ""})
    {{{cereal_serialize_body(variable_type_and_metadatas.metadatas) if has_metadatas else ""}
    }}
}};'''
        return out
    return variable_definition


def files():
    res = [
        register_set_variable_commands,
        register_set_variable_metadata_commands,
        VariableRegistries,
        AnyInput,
        AnyInputRef,
        AnyInputRefToConst,
        all_variable_includes,
        find_metadatas_in_string,
        variables_includes,
    ]
    for variable_types_and_metadatas in all_variable_types_and_metadatas():
        variable_definition = variable_definition_factory(
            variable_types_and_metadatas)
        variable_definition.__name__ = f"Variable_{strip_namespace(variable_types_and_metadatas.type)}"
        res.append(variable_definition)
    return res


if __name__ == '__main__':
    # HACK: Python doesn't allow us to import from a parent folder (e.g. tooling.generate_files)
    # So we need to add the path manually to sys.path
    import os
    import sys
    from pathlib import Path
    sys.path.append(os.path.join(
        Path(os.path.abspath(__file__)).parent.parent.parent.parent,
        "tooling")
    )
    # End of HACK

    import generate_files
    generate_files.generate(
        folder="generated",
        files=files(),
    )
    # print(files())
