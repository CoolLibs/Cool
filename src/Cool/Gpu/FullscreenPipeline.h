#pragma once
#include <Cool/Log/OptionalErrorMessage.h>
#include "Shader.h"
// #include "UniqueVAO.h"

namespace Cool {

// TODO(WebGPU) Turn this into a compute shader? removes the need for the dummy vao
class FullscreenPipeline {
public:
    FullscreenPipeline() = default;

    static void shut_down();

    /// Compiles the fragment shader source code and creates a new pipeline using this new shader.
    /// Returns an error message if the compilation failed.
    auto compile(std::string_view fragment_shader_source_code) -> OptionalErrorMessage;
    void reset() { _shader.reset(); }
    void draw() const;
    auto shader() const -> const std::optional<Shader>& { return _shader; }

private:
    std::optional<Shader> _shader{};
    // UniqueVAO             _dummy_vao{};
};

} // namespace Cool
