#pragma once
#if defined(COOL_OPENGL)

#include "Shader.h"

namespace Cool::OpenGL {

class FullscreenPipeline {
public:
    explicit FullscreenPipeline(std::string_view source);
    FullscreenPipeline(const FullscreenPipeline&) = delete;            // non-copyable because we store a handle to a VAO
    FullscreenPipeline& operator=(const FullscreenPipeline&) = delete; // non-copyable because we store a handle to a VAO
    ~FullscreenPipeline();

    void recompile(std::string_view source);

    auto shader() -> std::optional<Shader>& { return _shader; }
    void draw();

private:
    std::optional<Shader> _shader;
    GLuint                _dummy_vao_id{};
};

} // namespace Cool::OpenGL

#endif