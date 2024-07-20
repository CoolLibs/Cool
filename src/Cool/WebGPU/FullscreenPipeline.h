#pragma once
#include <webgpu/webgpu.hpp>
#include "BindGroup.hpp"
#include "BindGroupLayout.hpp"
#include "Buffer.h"
#include "Cool/Gpu/WebGPUContext.h"
#include "ShaderModule.h"
#include "WGPUUnique.h"
#include "glm/glm.hpp"

namespace Cool {

struct FullscreenPipeline_CreationArgs {
    ShaderModule_CreationArgs fragment_shader_module_creation_args;
    BindGroupLayout const*    extra_bind_group_layout = nullptr;
};

// TODO(WebGPU) Turn this into a compute shader? removes the need for the dummy vao. Problem: some effects need ddx() and ddy()
class FullscreenPipeline : public WGPUUnique<wgpu::RenderPipeline> {
public:
    explicit FullscreenPipeline(FullscreenPipeline_CreationArgs const& args);

    void draw(wgpu::RenderPassEncoder&, BindGroup const* extra_bind_group = nullptr) const;

    template<typename T>
    void set_uniform_with_offset(uint64_t offset, T const& value) const
    {
        if constexpr (std::is_same_v<T, bool>)
        {
            do_set_uniform_with_offset(offset, value ? 1 : 0); // Set bools as ints instead of bools, otherwise this crashes because WebGPU disallows bool uniforms.
        }
        else if constexpr (std::is_same_v<T, glm::mat3>)
        {
            do_set_uniform_with_offset(offset, glm::mat3x4{value}); // Needed for alignment reasons
        }
        else
        {
            do_set_uniform_with_offset(offset, value);
        }
    }
    void set_aspect_ratio_uniform(float aspect_ratio) const; // TODO(WebGPU) Remove? Get aspect ratio automatically from the render target?

private:
    template<typename T>
    void do_set_uniform_with_offset(uint64_t offset, T const& value) const
    {
        webgpu_context().queue.writeBuffer(_uniforms_buffer_fragment_shader, offset, &value, sizeof(value));
    }

private:
    Buffer    _uniforms_buffer_fragment_shader;
    Buffer    _uniforms_buffer_vertex_shader;
    BindGroup _bind_group;
    bool      _has_extra_bind_group;
};

} // namespace Cool
