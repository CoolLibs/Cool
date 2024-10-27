#include "ParticleSystem.h"
#include <imgui.h>
#include <cmath>
#include "Cool/Log/Debug.h"
#include "Mouse.hpp"

namespace Cool {

ParticleSystem::ParticleSystem(int dimension, ParticlesShadersCode const& shader_code, size_t particles_count)
    : _particles_count{particles_count}
    , _dimension(dimension)
#if !defined(COOL_PARTICLES_DISABLED_REASON)
    , _rendering_shader{}
    , _simulation_shader{64, shader_code.simulation}
    , _init_shader{64, shader_code.init}
#endif
{
    _rendering_shader.compile(shader_code.fragment);
    set_particles_count(_particles_count); // Will init all the particles attributes (by calling the init shader)
}

void ParticleSystem::render()
{
#if !defined(COOL_PARTICLES_DISABLED_REASON)
    bind_SSBOs();
    _rendering_shader.shader()->set_uniform("_size", (int)_particles_count);
    _rendering_shader.shader()->set_uniform("_first_frame", _first_frame);
    _rendering_shader.draw();
#endif
}

void ParticleSystem::update()
{
#if !defined(COOL_PARTICLES_DISABLED_REASON)
    bind_SSBOs();
    _simulation_shader.bind();
    _simulation_shader.set_uniform("_add_thing", mouse_pressed());
    _simulation_shader.set_uniform("_mouse_pos", mouse_pos());
    _simulation_shader.set_uniform("_aspect_ratio", std::sqrt(2.f));
    _simulation_shader.set_uniform("_first_frame", _first_frame);
    // Cool::Log::Debug::info("xdf", ImGui::GetIO().MouseDown[ImGuiMouseButton_Left] ? "oui" : "non");
    _simulation_shader.compute({_particles_count, _particles_count, 1});
    _first_frame = !_first_frame;
#endif
}

void ParticleSystem::set_simulation_shader(std::string const& shader_code)
{
#if !defined(COOL_PARTICLES_DISABLED_REASON)
    _simulation_shader = OpenGL::ComputeShader{64, shader_code};
#else
    std::ignore = shader_code;
#endif
}

void ParticleSystem::reset()
{
#if !defined(COOL_PARTICLES_DISABLED_REASON)
    bind_SSBOs();
    _init_shader.bind();
    _init_shader.compute({_particles_count, _particles_count, 1});
#endif
}

void ParticleSystem::set_particles_count(size_t particles_count)
{
    _particles_count = 400;
#if !defined(COOL_PARTICLES_DISABLED_REASON)
    bind_SSBOs();
    _concentration_a.upload_data(_particles_count * _particles_count, nullptr);
    _concentration_b.upload_data(_particles_count * _particles_count, nullptr);
    _concentration_a2.upload_data(_particles_count * _particles_count, nullptr);
    _concentration_b2.upload_data(_particles_count * _particles_count, nullptr);
    _init_shader.bind();
    _init_shader.compute({_particles_count, _particles_count, 1});
#else
    std::ignore = particles_count;
#endif
}

void ParticleSystem::bind_SSBOs()
{
#if !defined(COOL_PARTICLES_DISABLED_REASON)
    _concentration_a.bind();
    _concentration_b.bind();
    _concentration_a2.bind();
    _concentration_b2.bind();
#endif
}

} // namespace Cool
