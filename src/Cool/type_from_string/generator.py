# ---HOW TO---
# Modify `all_associations()` and `includes()`, then run this script.
# ------------


def all_associations():
    return {
        "bool":               ["bool"],
        "int":                ["int"],
        "float":              ["float",  "vec1"],
        "glm::vec2":          ["float2", "vec2"],
        "glm::vec3":          ["float3", "vec3"],
        "glm::vec4":          ["float4", "vec4"],
        "Cool::RgbColor":     ["RgbColor"],
        "Cool::Angle":        ["Angle"],
        "Cool::Direction2D":  ["Direction2D"],
        "Cool::Hue":          ["Hue"],
        # "Cool::ColorPalette": ["ColorPalette"],
    }


def includes():
    return f"""
#include <glm/glm.hpp>
#include <Cool/StrongTypes/RgbColor.h>
#include <Cool/StrongTypes/Angle.h>
#include <Cool/StrongTypes/Direction2D.h>
#include <Cool/StrongTypes/Hue.h>
"""


def string_to_type_associations():
    out = "\n"
    for key, values in all_associations().items():
        for value in values:
            out += f"""
template<>
struct from_impl<"{value}"> {{
    using type = {key};
}};
"""
    return out


def evaluate_function_template():
    out = "#define COOL_TFS_EVALUATE_FUNCTION_TEMPLATE(function_template, type_as_string, out_type, arguments) \\\n"
    out += "([&]() -> out_type { \\\n"
    is_first = True
    for key, values in all_associations().items():
        for value in values:
            out += f'{"else " if not is_first else ""}if ((type_as_string) == "{value}") return function_template<{key}>arguments;' + "\\\n"
            is_first = False

    out += f'else {{ throw std::runtime_error{{"Unknown type \\"" + std::string{{type_as_string}} + "\\""}};  }} \\\n}})()'
    return out


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
        files=[
            string_to_type_associations,
            includes,
            evaluate_function_template,
        ],
    )
