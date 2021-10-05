#pragma once
#if defined(COOL_OPENGL)

#include "Shader.h"

namespace Cool::OpenGL {

class FullscreenPipeline {
public:
    FullscreenPipeline(std::string_view fragment_shader_path);
    FullscreenPipeline(const FullscreenPipeline&) = delete;            // non-copyable because we store a handle to a VAO
    FullscreenPipeline& operator=(const FullscreenPipeline&) = delete; // non-copyable because we store a handle to a VAO
    ~FullscreenPipeline();

    void recompile_shader_from_file(std::string_view fragment_shader_path);
    void recompile_shader_from_source(std::string_view fragment_source_code);

    Shader& shader() { return _shader; }
    void    draw();

private:
    Shader _shader;
    GLuint _dummy_vao_id{};
};

} // namespace Cool::OpenGL

#endif