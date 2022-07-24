# ---HOW TO---

# ------------

from debug_options_generator import DebugOption, generate_debug_options


def all_debug_options():
    return [
        DebugOption(
            name_in_code="log_when_creating_icon",
            name_in_ui="Log when creating icon",
        ),
    ]


if __name__ == '__main__':
    generate_debug_options(
        output_folder="generated",
        namespace="Cool",
        debug_options=all_debug_options(),
    )
