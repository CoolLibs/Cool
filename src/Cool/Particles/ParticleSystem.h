#pragma once
#include <glpp/extended.hpp>
#include "Cool/Gpu/OpenGL/ComputeShader.h"
#include "Cool/Gpu/OpenGL/SSBO.h"
#include "Cool/Gpu/OpenGL/Shader.h"

namespace Cool {

struct ParticlesShadersCode {
    std::string simulation;
    std::string init;
    std::string vertex;
    std::string fragment;
};

class ParticleSystem {
public:
    explicit ParticleSystem(size_t particles_count = 500, ParticlesShadersCode const& = {});

    void render();
    void update();

    auto simulation_shader() -> OpenGL::ComputeShader& { return _simulation_shader; }
    auto simulation_shader() const -> OpenGL::ComputeShader const& { return _simulation_shader; }
    auto init_shader() -> OpenGL::ComputeShader& { return _init_shader; }
    auto init_shader() const -> OpenGL::ComputeShader const& { return _init_shader; }
    auto render_shader() -> OpenGL::Shader& { return _render_shader; }
    auto render_shader() const -> OpenGL::Shader const& { return _render_shader; }
    void set_simulation_shader(std::string const& shader_code);
    void reset();
    void set_particles_count(size_t particles_count);
    void set_particle_size(float particle_size);

private:
    SSBO<float> _positions{0};
    SSBO<float> _velocities{1};
    size_t      _particles_count{};
    SSBO<float> _sizes{2};
    SSBO<float> _lifetimes{3};

    OpenGL::Shader          _render_shader;
    glpp::UniqueVertexArray _render_vao{};
    glpp::UniqueBuffer      _render_vbo{};

    OpenGL::ComputeShader _simulation_shader;
    OpenGL::ComputeShader _init_shader;
};

} // namespace Cool
