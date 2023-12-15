#include "ParticleSystem.h"
#include <Cool/Random/Random.h>
#include "Cool/Gpu/OpenGL/ShaderModule.h"
#include "Cool/Gpu/ShaderKind.h"
#include "Cool/Particles/ParticleSystem.h"

namespace Cool {

ParticleSystem::ParticleSystem(size_t particles_count, ParticlesShadersCode const& shader_code, size_t const& dimension)
    : _particles_count{particles_count}
    , _dimension(dimension)
    , _render_shader{
          Cool::OpenGL::ShaderModule{Cool::ShaderDescription{
              .kind        = Cool::ShaderKind::Vertex,
              .source_code = shader_code.vertex,
          }},
          Cool::OpenGL::ShaderModule{Cool::ShaderDescription{
              .kind        = Cool::ShaderKind::Fragment,
              .source_code = shader_code.fragment,
          }}
      }
    , _simulation_shader{64, shader_code.simulation}
    , _init_shader{64, shader_code.init}
{
    set_particles_count(_particles_count);
    // _init_shader.bind();

    // _positions.upload_data(_particles_count * _dimension, nullptr);
    // _velocities.upload_data(_particles_count * _dimension, nullptr);
    // _sizes.upload_data(_particles_count, nullptr);
    // _lifetimes.upload_data(_particles_count, nullptr);
    // _colors.upload_data(_particles_count * 4, nullptr);
    // _init_shader.compute({_particles_count, 1, 1});

    glpp::bind_vertex_array(_render_vao);
    glpp::bind_vertex_buffer(_render_vbo);
    glpp::set_vertex_buffer_attribute(_render_vbo, 0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);                          // Vertices positions
    glpp::set_vertex_buffer_attribute(_render_vbo, 1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))); // Vertices UVs
    glpp::set_vertex_buffer_data(
        _render_vbo, glpp::DataAccessFrequency::Static,
        std::array{
            -1.f, -1.f, 0.0f, 0.0f,
            +1.f, -1.f, 1.0f, 0.0f,
            +1.f, +1.f, 1.0f, 1.0f,

            -1.f, -1.f, 0.0f, 0.0f,
            +1.f, +1.f, 1.0f, 1.0f,
            -1.f, +1.f, 0.0f, 1.0f
        }
    );
}

void ParticleSystem::render()
{
    bind_SSBOs();
    _render_shader.bind();
    glpp::bind_vertex_array(_render_vao);
    glpp::draw_arrays_instanced(_render_vao, glpp::PrimitiveDrawMode::Triangles, 0, 6, static_cast<GLsizei>(_particles_count));
}

void ParticleSystem::update()
{
    bind_SSBOs();
    _simulation_shader.bind();
    _simulation_shader.compute({_particles_count, 1, 1});
}

void ParticleSystem::set_simulation_shader(std::string const& shader_code)
{
    _simulation_shader = OpenGL::ComputeShader{64, shader_code};
}

void ParticleSystem::reset()
{
    bind_SSBOs();
    _init_shader.bind();
    _init_shader.compute({_particles_count, 1, 1});
}

void ParticleSystem::set_particles_count(size_t _particles_count)
{
    bind_SSBOs();
    _init_shader.bind();
    _positions.upload_data(_particles_count * 2, nullptr);
    _velocities.upload_data(_particles_count * 2, nullptr);
    _sizes.upload_data(_particles_count, nullptr);
    _colors.upload_data(_particles_count * 4, nullptr);
    _init_shader.compute({_particles_count, 1, 1});
}

void ParticleSystem::set_particle_size(float particle_size)
{
    _sizes.bind();
}

void ParticleSystem::bind_SSBOs()
{
    _positions.bind();
    _velocities.bind();
    _sizes.bind();
    _colors.bind();
}

} // namespace Cool