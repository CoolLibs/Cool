#pragma once
#include <glpp/extended.hpp>
#include "Cool/Gpu/OpenGL/SSBO.h"
#include "Cool/Gpu/OpenGL/Shader.h"

namespace Cool {

class ParticleSystem {
public:
    ParticleSystem();

    void render();

private:
    SSBO<float> _positions{0};
    size_t      _particles_count{};

    OpenGL::Shader          _render_shader{};
    glpp::UniqueVertexArray _render_vao{};
    glpp::UniqueBuffer      _render_vbo{};
};

} // namespace Cool