# ---HOW TO---

# ------------

from debug_options_generator import DebugOption, generate_debug_options


def all_debug_options():
    return [
        DebugOption(
            name_in_code="log_when_creating_icon",
            name_in_ui="Log when creating icon",
        ),
        DebugOption(
            name_in_code="log_number_of_threads_in_the_thread_pool",
            name_in_ui="Log the number of threads in the thread pool",
        ),
        DebugOption(
            name_in_code="log_opengl_info",
            name_in_ui="Log OpenGL info",
        ),
    ]


if __name__ == '__main__':
    generate_debug_options(
        output_folder="generated",
        namespace="Cool",
        cache_file_name="cache--debug-options-cool",
        debug_options=all_debug_options(),
    )
