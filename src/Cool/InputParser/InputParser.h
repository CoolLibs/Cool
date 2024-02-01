#include <Cool/Dependencies/AnyInput.h>

namespace Cool {
// TODO(Variables) Rename file

/// Defines the uniform in shader code; e.g. something like `uniform float name;` (or something more complex for some types like Gradient).
auto gen_input_shader_code(AnyInput const&) -> std::string;

/// Defines the uniform in shader code; e.g. something like `uniform float name;` (or something more complex for some types like Gradient).
auto gen_input_shader_code(AnyInput const&, std::string_view name /* Allows us to use a different name than the input's user-facing name if we want to */) -> std::string;

} // namespace Cool