#pragma once
#if defined(COOL_OPENGL)

#include "Shader.h"
#include "UniqueVAO.h"

namespace Cool::OpenGL {

class FullscreenPipeline {
public:
    FullscreenPipeline() = default;
    /**
     * @param name The name that will be displayed in the error message if the compilation fails 
     */
    FullscreenPipeline(std::string_view fragment_shader_source_code, std::string_view name);

    /**
     * @brief Compiles the fragment shader source code and creates a new pipeline using this new shader.
     * @param name The name that will be displayed in the error message if the compilation fails 
     */
    void compile(std::string_view fragment_shader_source_code, std::string_view name);
    void draw() const;
    auto shader() const -> const std::optional<Shader>& { return _shader; }

private:
    std::optional<Shader> _shader{};
    UniqueVAO             _dummy_vao{};
};

} // namespace Cool::OpenGL

#endif