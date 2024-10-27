#pragma once
#include <glpp/extended.hpp>
#include "Cool/Gpu/OpenGL/ComputeShader.h"
#include "Cool/Gpu/OpenGL/FullscreenPipeline.h"
#include "Cool/Gpu/OpenGL/SSBO.h"
#include "Cool/Gpu/OpenGL/Shader.h"

namespace Cool {

struct ParticlesShadersCode {
    std::string simulation{};
    std::string init{};
    std::string vertex{};
    std::string fragment{};
};

class ParticleSystem {
public:
    /// `dimension` should be 2 for 2D particles, and 3 for 3D particles
    explicit ParticleSystem(int dimension, ParticlesShadersCode const&, size_t particles_count = 500);

    void render();
    void update();

#if !defined(COOL_PARTICLES_DISABLED_REASON)
    auto simulation_shader() -> OpenGL::ComputeShader& { return _simulation_shader; }
    auto simulation_shader() const -> OpenGL::ComputeShader const& { return _simulation_shader; }
    auto init_shader() -> OpenGL::ComputeShader& { return _init_shader; }
    auto init_shader() const -> OpenGL::ComputeShader const& { return _init_shader; }

    auto render_shader() -> OpenGL::Shader& { return *_rendering_shader.shader(); }
    auto render_shader() const -> OpenGL::Shader const& { return *_rendering_shader.shader(); }
#endif
    auto dimension() const -> int { return _dimension; }

    void set_simulation_shader(std::string const& shader_code);
    void reset();
    void set_particles_count(size_t particles_count);

    auto particles_count() const -> size_t { return _particles_count; }

private:
    void bind_SSBOs();

private:
    size_t _particles_count{};
    int    _dimension{}; // 2 for 2D particles, and 3 for 3D particles
#if !defined(COOL_PARTICLES_DISABLED_REASON)
    SSBO<float> _concentration_a{0};
    SSBO<float> _concentration_b{1};
    SSBO<float> _concentration_a2{2};
    SSBO<float> _concentration_b2{3};
    bool        _first_frame{true};

    OpenGL::FullscreenPipeline _rendering_shader;

    OpenGL::ComputeShader _simulation_shader;
    OpenGL::ComputeShader _init_shader;
#endif
};

} // namespace Cool
