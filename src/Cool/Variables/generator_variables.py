# ---HOW TO---
# Create a function with the same name as the file you want to generate
# that returns a string with the content you want to generate.
#
# Then add your function to the call to `generate()` at the end of the file.
#
# You can use `all_variable_types()` to get all the variable types we use in CoolLab.
# ------------

from dataclasses import dataclass, field
from typing import List

if True:
    # HACK: Python doesn't allow us to import from a parent folder (e.g. tooling.generate_files)
    # So we need to add the path manually to sys.path
    import os
    import sys
    from pathlib import Path
    sys.path.append(os.path.join(
        Path(os.path.abspath(__file__)).parent.parent,
        "ColorSpaces")
    )
    # End of HACK
    import generator_colors


@dataclass
class VariableMetadata:
    field_name: str
    pretty_name: str
    type: str
    default_value: str
    name_in_shader: str = ""


@dataclass
class VariableDescription:
    input_type: List[str]  # Type that comes in an INPUT declaration
    cpp_type: str  # Type used in C++ to store the type
    glsl_type: str  # Raw glsl used to store the type
    include: str = ""  # File containing the C++ type
    metadatas: list[VariableMetadata] = field(
        default_factory=lambda: [])
    do_generate_get_default_metadata: bool = True
    # For example a Gradient variable can't be sent as a simple uniform to a shader; that's why we generate some to inject it into the shader.
    requires_shader_code_generation: bool = False


def all_variable_descriptions():

    return [
        VariableDescription(
            input_type=["bool"],
            cpp_type="bool",
            glsl_type="bool",
            metadatas=[],
            requires_shader_code_generation=False,
        ),
        VariableDescription(
            input_type=["int"],
            cpp_type="int",
            glsl_type="int",
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
            ],
            requires_shader_code_generation=False,
        ),
        VariableDescription(
            input_type=["float"],
            cpp_type="float",
            glsl_type="float",
            metadatas=[
                VariableMetadata(
                    name_in_shader="min",  # TODO(JF) Remove this
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
                VariableMetadata(
                    name_in_shader="bounded",
                    field_name="bounded",
                    pretty_name="Bounded",
                    type="bool",
                    default_value="false",
                ),
                VariableMetadata(
                    name_in_shader="drag_speed",
                    field_name="drag_speed",
                    pretty_name="Drag speed",
                    type="float",
                    default_value="0.01f",
                ),
            ],
            requires_shader_code_generation=False,
        ),
        VariableDescription(
            input_type=["UV", "Point2D"],
            cpp_type="Cool::Point2D",
            glsl_type="vec2",
            include="<Cool/StrongTypes/Point2D.h>",
            metadatas=[],
            requires_shader_code_generation=False,
        ),
        VariableDescription(
            input_type=["vec2"],
            cpp_type="glm::vec2",
            glsl_type="vec2",
            include="<glm/glm.hpp>",
            metadatas=[
                VariableMetadata(
                    name_in_shader="drag_speed",
                    field_name="drag_speed",
                    pretty_name="Drag speed",
                    type="float",
                    default_value="0.01f",
                ),
            ],
            requires_shader_code_generation=False,
        ),
        VariableDescription(
            input_type=["vec3"],
            cpp_type="glm::vec3",
            glsl_type="vec3",
            include="<glm/glm.hpp>",
            metadatas=[
                VariableMetadata(
                    name_in_shader="drag_speed",
                    field_name="drag_speed",
                    pretty_name="Drag speed",
                    type="float",
                    default_value="0.01f",
                ),
            ],
            requires_shader_code_generation=False,
        ),
        VariableDescription(
            input_type=["vec4"],
            cpp_type="glm::vec4",
            glsl_type="vec4",
            include="<glm/glm.hpp>",
            metadatas=[
                VariableMetadata(
                    name_in_shader="drag_speed",
                    field_name="drag_speed",
                    pretty_name="Drag speed",
                    type="float",
                    default_value="0.01f",
                ),
            ],
            requires_shader_code_generation=False,
        ),
        VariableDescription(
            input_type=list(map(lambda x: x.name_in_code,
                                generator_colors.color_spaces())),
            cpp_type="Cool::Color",
            glsl_type="vec3",
            include="<Cool/StrongTypes/Color.h>",
            metadatas=[
                VariableMetadata(
                    name_in_shader="hdr",
                    field_name="is_hdr",
                    pretty_name="Is HDR",
                    type="bool",
                    default_value="false",
                ),
            ],
            do_generate_get_default_metadata=False,
            requires_shader_code_generation=False,
        ),
        VariableDescription(
            input_type=list(generator_colors.color_and_alpha_spaces_names()),
            cpp_type="Cool::ColorAndAlpha",
            glsl_type="vec4",
            include="<Cool/StrongTypes/ColorAndAlpha.h>",
            metadatas=[
                VariableMetadata(
                    name_in_shader="hdr",
                    field_name="is_hdr",
                    pretty_name="Is HDR",
                    type="bool",
                    default_value="false",
                ),
            ],
            do_generate_get_default_metadata=False,
            requires_shader_code_generation=False,
        ),
        VariableDescription(
            input_type=["Camera"],
            cpp_type="Cool::Camera",
            # NB: we would probably need to create a Camera struct in glsl if we really intended to use this variable in shaders. (Which we will definitely do at some point instead of having one single global camera)
            glsl_type="mat4",
            include="<Cool/Camera/Camera.h>",
            metadatas=[],
            requires_shader_code_generation=False,
        ),
        VariableDescription(
            input_type=["Angle"],
            cpp_type="Cool::Angle",
            glsl_type="float",
            include="<Cool/StrongTypes/Angle.h>",
            metadatas=[],
            requires_shader_code_generation=False,
        ),
        VariableDescription(
            input_type=["Direction2D"],
            cpp_type="Cool::Direction2D",
            glsl_type="vec2",
            include="<Cool/StrongTypes/Direction2D.h>",
            metadatas=[],
            requires_shader_code_generation=False,
        ),
        VariableDescription(
            input_type=["Hue"],
            cpp_type="Cool::Hue",
            glsl_type="float",
            include="<Cool/StrongTypes/Hue.h>",
            metadatas=[],
            requires_shader_code_generation=False,
        ),
        VariableDescription(
            input_type=["ColorPalette"],
            cpp_type="Cool::ColorPalette",
            glsl_type="NO TYPE THIS IS A FUNCTION",
            include="<Cool/StrongTypes/ColorPalette.h>",
            metadatas=[
                VariableMetadata(
                    name_in_shader="hdr",
                    field_name="is_hdr",
                    pretty_name="Is HDR",
                    type="bool",
                    default_value="true",
                ),
            ],
            requires_shader_code_generation=True,
        ),
        VariableDescription(
            input_type=["Gradient"],
            cpp_type="Cool::Gradient",
            glsl_type="NO TYPE THIS IS A FUNCTION",
            include="<Cool/StrongTypes/Gradient.h>",
            metadatas=[
                VariableMetadata(
                    name_in_shader="hdr",
                    field_name="is_hdr",
                    pretty_name="Is HDR",
                    type="bool",
                    default_value="true",
                ),
                VariableMetadata(
                    field_name="randomize_new_marks_colors",
                    pretty_name="Randomize new marks' colors",
                    type="bool",
                    default_value="true",
                ), ],
            requires_shader_code_generation=True,
        ),
    ]


def all_variable_types():
    return map(lambda desc: desc.cpp_type,
               all_variable_descriptions())


def all_types_representations_as_strings():
    return {desc.cpp_type: desc.input_type
            for desc in all_variable_descriptions()}


def strip_namespace(variable_type):
    return variable_type[variable_type.rfind(":")+1:]


def all_variable_types_without_namespaces():
    return map(lambda variable_type: strip_namespace(variable_type),
               all_variable_types())


def all_variable_descriptions_without_namespaces():
    def strip_namespace_on_variable_type(desc):
        desc.type = strip_namespace(desc.type)
        return desc
    return map(lambda variable_type_and_metadatas: strip_namespace_on_variable_type(variable_type_and_metadatas),
               all_variable_descriptions())


def all_variable_includes():
    return f"""
            #include <Cool/Camera/Camera.h>
            #include <Cool/Variables/Variables.h>
            """


def all_includes():
    return map(lambda var_desc: var_desc.include,
               all_variable_descriptions())


def all_non_empty_includes():
    return filter(lambda include: include != "",
                  all_includes())


def all_types_includes():
    return "\n".join(map(
        lambda path: f"#include {path}",
        all_non_empty_includes()
    ))


# def register_set_variable_commands():
#     commands = ""
#     reversible_commands = ""
#     for variable_type in all_variable_types():
#         commands += f"LAB_REGISTER_COMMAND(Lab::Command_SetVariable<{variable_type}>)\n"
#         reversible_commands += f"LAB_REGISTER_REVERSIBLE_COMMAND(Lab::ReversibleCommand_SetVariable<{variable_type}>)\n"
#     return f"""
# {commands}
# {reversible_commands}
# """


# def register_set_variable_metadata_commands():
#     out = "\n"
#     for variable_type in all_variable_types():
#         out += f"LAB_REGISTER_COMMAND(Lab::Command_SetVariableMetadata<{variable_type}>)\n"
#     return out


def VariableRegistries():
    return "\n" + "using VariableRegistries = reg::Registries<\n" + ",\n".join(
        map(lambda var_type: f"reg::Registry<Cool::Variable<{var_type}>>", all_variable_types())) + "\n>;"


def AnyInput():
    return "\n" + "using AnyInput = std::variant<\n" + ",\n".join(
        map(lambda var_type: f"Input<{var_type}>", all_variable_types())) + "\n>;"


def AnyInputDefinition():
    return "\n" + "using AnyInputDefinition = std::variant<\n" + ",\n".join(
        map(lambda var_type: f"InputDefinition<{var_type}>", all_variable_types())) + "\n>;"


def AnyVariable():
    return "\n" + "using AnyVariable = std::variant<\n" + ",\n".join(
        map(lambda var_type: f"Variable<{var_type}>", all_variable_types())) + "\n>;"


def AnyInputRef():
    return "\n" + "using AnyInputRef = std::variant<\n" + ",\n".join(
        map(lambda var_type: f"std::reference_wrapper<Input<{var_type}>>", all_variable_types())) + "\n>;"


def AnyInputRefToConst():
    return "\n" + "using AnyInputRefToConst = std::variant<\n" + ",\n".join(
        map(lambda var_type: f"std::reference_wrapper<const Input<{var_type}>>", all_variable_types())) + "\n>;"


def find_metadatas_in_string():
    out = ""
    for variable_types_and_metadatas in all_variable_descriptions():
        if variable_types_and_metadatas.do_generate_get_default_metadata == True:
            out += f'''
                template<>
                auto get_default_metadata(std::string_view{" key_values" if variable_types_and_metadatas.metadatas else ""}) -> Cool::VariableMetadata<{variable_types_and_metadatas.cpp_type}>
                {{
                    Cool::VariableMetadata<{variable_types_and_metadatas.cpp_type}> metadata{{}};
                '''
            for variable_metadatas in variable_types_and_metadatas.metadatas:
                out += f'''
                    const auto {variable_metadatas.field_name} = Cool::String::find_value_for_given_key<{variable_metadatas.type}>(key_values, "{variable_metadatas.name_in_shader}");
                    if ({variable_metadatas.field_name})
                    {{
                        metadata.{variable_metadatas.field_name} = *{variable_metadatas.field_name};
                    }}
                    '''
            out += "return metadata;\n}\n"
    return out


def variables_includes():
    out = "\n"
    for variable_type in all_variable_types_without_namespaces():
        out += f"#include <Cool/Variables/Variable_{variable_type}.h>\n"
    return out


def glsl_type():
    return "\n\n".join(
        map(lambda desc: f'''
            template<>
            auto glsl_type<{desc.cpp_type}>() -> std::string {{ 
                return "{desc.glsl_type}";
            }}
            ''', all_variable_descriptions()))


def cereal_make_nvp(metadatas: List[VariableMetadata]):
    return ",\n".join(map(lambda meta:
                          f'cereal::make_nvp("{meta.pretty_name}", {meta.field_name})', metadatas)
                      )


def cereal_serialize_body(metadatas: List[VariableMetadata]):
    return f'''
        archive(
{cereal_make_nvp(metadatas)}
        );'''


def metadatas_definitions(metadatas: List[VariableMetadata]):
    return "\n".join(map(
        lambda metadata:
            f"{metadata.type} {metadata.field_name}{{{metadata.default_value}}};",
            metadatas))


def variable_definition_factory(variable_type_and_metadatas):
    def variable_definition():
        has_metadatas = len(variable_type_and_metadatas.metadatas) > 0
        return f'''
            template<>
            struct VariableMetadata<{variable_type_and_metadatas.cpp_type}> {{
                {metadatas_definitions(variable_type_and_metadatas.metadatas)}

                friend auto operator<=>(const VariableMetadata<{variable_type_and_metadatas.cpp_type}>&, const VariableMetadata<{variable_type_and_metadatas.cpp_type}>&) = default;

            private:
                // Serialisation
                friend class cereal::access;
                template<class Archive>
                void serialize(Archive&{" archive" if has_metadatas else ""})
                {{{cereal_serialize_body(variable_type_and_metadatas.metadatas) if has_metadatas else ""}
                }}
            }};'''
    return variable_definition


def requires_shader_code_generation():
    return "\n\n".join(
        map(lambda desc: f'''
            if constexpr (std::is_same_v<T, {desc.cpp_type}>)
                return {"true" if desc.requires_shader_code_generation else "false"};
            ''', all_variable_descriptions()))


def files():
    res = [
        # register_set_variable_commands,
        # register_set_variable_metadata_commands,
        VariableRegistries,
        AnyInput,
        AnyInputDefinition,
        AnyVariable,
        AnyInputRef,
        AnyInputRefToConst,
        all_variable_includes,
        find_metadatas_in_string,
        variables_includes,
        glsl_type,
        requires_shader_code_generation,
    ]
    for variable_types_and_metadatas in all_variable_descriptions():
        variable_definition = variable_definition_factory(
            variable_types_and_metadatas)
        variable_definition.__name__ = f"Variable_{strip_namespace(variable_types_and_metadatas.cpp_type)}"
        res.append(variable_definition)
    return res


def main():
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

    # HACK: Python doesn't allow us to import from a parent folder
    # So we need to load it manually
    from os import path
    from pathlib import Path
    from importlib.machinery import SourceFileLoader
    tfs = SourceFileLoader(
        "tfs",
        path.join(Path(path.abspath(__file__)).parent.parent, "type_from_string/generator_tfs.py")).load_module()
    # End of HACK
    tfs.main(
        all_types_representations_as_strings(),
        all_types_includes(),
    )


if __name__ == '__main__':
    main()
