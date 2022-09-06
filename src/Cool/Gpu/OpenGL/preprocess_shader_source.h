#pragma once
#if defined(COOL_OPENGL)

namespace Cool::OpenGL {

std::string preprocess_shader_source(std::string_view source);

} // namespace Cool::OpenGL

#endif