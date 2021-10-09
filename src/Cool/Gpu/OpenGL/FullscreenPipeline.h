#pragma once
#if defined(COOL_OPENGL)

#include "Shader.h"

namespace Cool::OpenGL {

class FullscreenPipeline {
public:
    FullscreenPipeline();
    explicit FullscreenPipeline(std::string_view fragment_shader_source_code);
    FullscreenPipeline(const FullscreenPipeline&) = delete;            // non-copyable because we store a handle to a VAO
    FullscreenPipeline& operator=(const FullscreenPipeline&) = delete; // non-copyable because we store a handle to a VAO
    ~FullscreenPipeline();

    /**
     * @brief Compiles the fragment shader source code and creates a new pipeline using this new shader.
     */
    void compile(std::string_view fragment_shader_source_code);

    auto shader() -> std::optional<Shader>& { return _shader; }
    void draw();

private:
    std::optional<Shader> _shader{};
    GLuint                _dummy_vao_id{};
};

} // namespace Cool::OpenGL

#endif