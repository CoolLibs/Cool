#pragma once
#if defined(COOL_OPENGL)

#include "../ShaderSource.h"
#include "Shader.h"

namespace Cool::OpenGL {

class FullscreenPipeline {
public:
    explicit FullscreenPipeline(const ShaderSource& source);
    FullscreenPipeline(const FullscreenPipeline&) = delete;            // non-copyable because we store a handle to a VAO
    FullscreenPipeline& operator=(const FullscreenPipeline&) = delete; // non-copyable because we store a handle to a VAO
    ~FullscreenPipeline();

    void recompile(const ShaderSource& source);

    Shader& shader() { return _shader; }
    void    draw();

private:
    Shader _shader;
    GLuint _dummy_vao_id{};
};

} // namespace Cool::OpenGL

#endif