
from dataclasses import dataclass


@dataclass
class ColorSpace:
    name_in_code: str
    user_facing_name: str


@dataclass
class AlphaSpace:
    name_in_code: str
    user_facing_name: str


def color_spaces():
    return [
        ColorSpace(
            name_in_code="CIELAB",
            user_facing_name="CIELAB",
        ),
        ColorSpace(
            name_in_code="LinearRGB",
            user_facing_name="Linear RGB",
        ),
        ColorSpace(
            name_in_code="sRGB",
            user_facing_name="sRGB",
        ),
        ColorSpace(
            name_in_code="HSLuv",
            user_facing_name="HSLuv",
        ),
    ]


def alpha_spaces():
    return [
        AlphaSpace(
            name_in_code="PremultipliedA",
            user_facing_name="Premultiplied Alpha",
        ),
        AlphaSpace(
            name_in_code="StraightA",
            user_facing_name="Straight Alpha",
        ),
    ]


def color_and_alpha_spaces_names():
    from itertools import product
    return map(lambda x: f"{x[0].name_in_code}_{x[1].name_in_code}",
               product(color_spaces(), alpha_spaces()))


def color_space_enum():
    return ",\n".join(map(lambda x: x.name_in_code, color_spaces()))


def color_and_alpha_space_enum():
    return ",\n".join(color_and_alpha_spaces_names())


def parse_color_space():
    res = ""
    for col in color_spaces():
        res += f'if (text == "{col.name_in_code}") return ColorSpace::{col.name_in_code};\n'
    return res


def parse_color_and_alpha_space():
    res = ""
    for space in color_and_alpha_spaces_names():
        res += f'if (text == "{space}") return ColorAndAlphaSpace::{space};\n'
    return res


def declare_color_getters():
    return "\n".join(map(lambda x: f"[[nodiscard]] auto as_{x.name_in_code}() const -> glm::vec3;", color_spaces()))


def conversion_from_srgb(space: str):
    return f"{space}_from_sRGB" if space != "sRGB" else ""


def define_color_getters():
    return "\n".join(map(lambda x: f"""
    auto Color::as_{x.name_in_code}() const -> glm::vec3
{{
    return {conversion_from_srgb(x.name_in_code)}(_srgb);
}}
""", color_spaces()))


def declare_color_and_alpha_getters():
    return "\n".join(map(lambda x: f"[[nodiscard]] auto as_{x}() const -> glm::vec4;", color_and_alpha_spaces_names()))


def define_color_and_alpha_getters():
    res = ""
    for col in color_spaces():
        res += f"""
        auto ColorAndAlpha::as_{col.name_in_code}_StraightA() const -> glm::vec4
{{
    return {{
        {conversion_from_srgb(col.name_in_code)}(glm::vec3{{
            _srgb_straight.r,
            _srgb_straight.g,
            _srgb_straight.b,
        }}),
        _srgb_straight.a,
    }};
}}"""
        res += f"""
        auto ColorAndAlpha::as_{col.name_in_code}_PremultipliedA() const -> glm::vec4
{{
    return {{
        {conversion_from_srgb(col.name_in_code)}(glm::vec3{{
            _srgb_straight.r,
            _srgb_straight.g,
            _srgb_straight.b,
        }}) * _srgb_straight.a,
        _srgb_straight.a,
    }};
}}"""
    return res


def convert_col_as():
    return '\n'.join(map(lambda x: f"""
        case Cool::ColorSpace::{x.name_in_code}:
            return col.as_{x.name_in_code}();
    """, color_spaces()))


def convert_col_and_alpha_as():
    return '\n'.join(map(lambda space: f"""
        case Cool::ColorAndAlphaSpace::{space}:
            return col.as_{space}();
    """, color_and_alpha_spaces_names()))


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
    sys.path.append(os.path.join(
        Path(os.path.abspath(__file__)).parent.parent,
        "Variables")
    )
    # End of HACK
    import generate_files
    import generator_variables

    generator_variables.main()

    generate_files.generate(
        folder="generated",
        files=[
            color_space_enum,
            color_and_alpha_space_enum,
            parse_color_space,
            parse_color_and_alpha_space,
            declare_color_getters,
            define_color_getters,
            declare_color_and_alpha_getters,
            define_color_and_alpha_getters,
            convert_col_as,
            convert_col_and_alpha_as,
        ],
    )
