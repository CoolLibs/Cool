#pragma once
#if defined(COOL_OPENGL)

#include <tl/expected.hpp>

namespace Cool::OpenGL {

/// Preprocesses the source code (replaces `#include` with the content of the corresponding file, etc.) and returns it.
/// Or returns an error (e.g. if an #included file was not found).
tl::expected<std::string, std::string> preprocess_shader_source(std::string_view source);

} // namespace Cool::OpenGL

#endif