#pragma once
#if defined(COOL_OPENGL)

#include "Shader.h"
#include "UniqueVAO.h"

namespace Cool::OpenGL {

class FullscreenPipeline {
public:
    FullscreenPipeline() = default;
    explicit FullscreenPipeline(std::string_view fragment_shader_source_code);

    /**
     * @brief Compiles the fragment shader source code and creates a new pipeline using this new shader.
     */
    void compile(std::string_view fragment_shader_source_code);
    void draw();
    auto shader() -> std::optional<Shader>& { return _shader; }

private:
    std::optional<Shader> _shader{};
    UniqueVAO             _dummy_vao{};
};

} // namespace Cool::OpenGL

#endif