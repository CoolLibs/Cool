#pragma once
#if defined(COOL_OPENGL)
#include "Cool/Log/ErrorMessage.hpp"
#include "Shader.h"
#include "UniqueVAO.h"
#include "tl/expected.hpp"

namespace Cool::OpenGL {

class FullscreenPipeline {
public:
    FullscreenPipeline() = default;

    /// Compiles the fragment shader source code and creates a new pipeline using this new shader.
    /// Returns an error message if the compilation failed.
    auto compile(std::string_view fragment_shader_source_code) -> tl::expected<void, ErrorMessage>;
    void reset() { _shader.reset(); }
    void draw() const;
    auto shader() const -> const std::optional<Shader>& { return _shader; }

private:
    std::optional<Shader> _shader{};
    UniqueVAO             _dummy_vao{};
};

} // namespace Cool::OpenGL

#endif