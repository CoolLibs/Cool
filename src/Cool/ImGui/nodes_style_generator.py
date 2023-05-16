
from pipe import *
from dataclasses import dataclass
from typing import List


def imgui_col_enum() -> str:
    import os
    from pathlib import Path
    path = os.path.join(Path(__file__).parent.parent.parent.parent,
                        "lib/imgui-node-editor/imgui_node_editor.h")

    with open(path, 'r') as file:
        content = file.read()
    begin = """enum StyleColor
{"""
    begin_idx = content.find(begin)
    end_idx = content.find("};", begin_idx)
    return content[begin_idx + len(begin): end_idx]


@dataclass
class ColorElement:
    id: str
    comment: str | None


def parse_color_elements() -> List[ColorElement]:
    def find_comment(line: str):
        index = line.find('//')
        if index == -1:
            return None
        return line[index + 2:].strip()

    def parse_one_line(line: str):
        return ColorElement(
            id=line.split()[0].strip().removesuffix(','),
            comment=find_comment(line),
        )

    res = list(
        imgui_col_enum().split('\n')
        | where(lambda l: l != "")
        | map(parse_one_line)
        | where(lambda x: x.id != "StyleColor_Count")
        # Don't register SelNodeBorder because it is hardcoded to be the color of the category of the node
        | where(lambda x: x.id != "StyleColor_SelNodeBorder")
    )

    return res


def register_all_imnodes_color_elements():
    elems = parse_color_elements()
    res = ""
    for elem in elems:
        res += f'''
config.register_element({{
    "Nodes_{elem.id.removeprefix("StyleColor_")}",
    []()
    {{
        return ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::{elem.id}];
    }},
    [](ImVec4 const& color)
    {{
        ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::{elem.id}] = color;
    }},
    "{elem.comment or ""}",
}});
'''
    return res


def main():
    import os
    from pathlib import Path
    from importlib.machinery import SourceFileLoader
    generate_files = SourceFileLoader(
        "generate_files",
        os.path.join(Path(os.path.abspath(__file__)
                          ).parent.parent.parent.parent, "tooling", "generate_files.py")).load_module()
    generate_files.generate(
        folder="generated_style_nodes",
        files=[
            register_all_imnodes_color_elements,
        ])


if __name__ == '__main__':
    main()
