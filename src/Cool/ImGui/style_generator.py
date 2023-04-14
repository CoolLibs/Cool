from dataclasses import dataclass


@dataclass
class ColorElement:
    name_in_code: str
    name_in_ui: str
    description: str

floating_button_description = 'Small buttons that overlap the View'

def all_color_elements():
    return [
        ColorElement(
            name_in_code='toggle',
            name_in_ui='Toggle',
            description='',
        ),
        ColorElement(
            name_in_code='toggle_hovered',
            name_in_ui='ToggleHovered',
            description='',
        ),
        ColorElement(
            name_in_code='toggle_bg',
            name_in_ui='ToggleBg',
            description='',
        ),
        ColorElement(
            name_in_code='toggle_bg_hovered',
            name_in_ui='ToggleBgHovered',
            description='',
        ),
        ColorElement(
            name_in_code='checkbox_button',
            name_in_ui='CheckboxButton',
            description='',
        ),
        ColorElement(
            name_in_code='checkbox_button_hovered',
            name_in_ui='CheckboxButtonHovered',
            description='',
        ),
        ColorElement(
            name_in_code='checkbox_button_active',
            name_in_ui='CheckboxButtonActive',
            description='',
        ),
        ColorElement(
            name_in_code='floating_button',
            name_in_ui='FloatingButton',
            description=floating_button_description,
        ),
        ColorElement(
            name_in_code='floating_button_hovered',
            name_in_ui='FloatingButtonHovered',
            description=floating_button_description,
        ),
        ColorElement(
            name_in_code='floating_button_active',
            name_in_ui='FloatingButtonActive',
            description=floating_button_description,
        ),
        ColorElement(
            name_in_code='buttons_separator',
            name_in_ui='ButtonsSeparator',
            description='Separator used when we glue several buttons together without any spacing between them (e.g. the timeline buttons).',
        ),
        ColorElement(
            name_in_code='highlight_items',
            name_in_ui='HighlightItems',
            description='Used for example when hovering some error messages, to highlight the part of the UI that needs to be used in order to fix the error message. Use the "Test Message Console" debug option to send such an error message.',
        ),
        ColorElement(
            name_in_code='gradient_mark',
            name_in_ui='GradientMark',
            description='',
        ),
        ColorElement(
            name_in_code='gradient_mark_hovered',
            name_in_ui='GradientMarkHovered',
            description='',
        ),
        ColorElement(
            name_in_code='gradient_mark_selected',
            name_in_ui='GradientMarkSelected',
            description='',
        ),
        ColorElement(
            name_in_code='gradient_mark_selected_hovered',
            name_in_ui='GradientMarkSelectedHovered',
            description='',
        ),
    ]


def style_colors():
    def declaration(element: ColorElement):
        return f'ImVec4 {element.name_in_code}{{0.f, 0.f, 0.f, 1.f}}; {"// " if element.description else ""}{element.description}'
    return '\n'.join(map(declaration, all_color_elements()))


def register_elements():
    def escape_quotes(string: str):
        return string.replace('"', '\\"')

    def register(element: ColorElement):
        return f'''
        config.register_element(ImStyleEd::Element{{
            .name        = "{element.name_in_ui}",
            .get_color   = []() {{ return ImGuiExtras::GetStyle().{element.name_in_code}; }},
            .set_color   = [](ImVec4 const& color) {{ ImGuiExtras::GetStyle().{element.name_in_code} = color; }},
            .description = "{escape_quotes(element.description)}",
        }});
    '''
    return '\n'.join(map(register, all_color_elements()))


if __name__ == '__main__':
    import os
    from pathlib import Path
    from importlib.machinery import SourceFileLoader
    generate_files = SourceFileLoader(
        'generate_files',
        os.path.join(Path(os.path.abspath(__file__)
                          ).parent.parent.parent.parent, 'tooling', 'generate_files.py')).load_module()
    generate_files.generate(
        folder="generated_style",
        files=[
            style_colors,
            register_elements,
        ],
    )
