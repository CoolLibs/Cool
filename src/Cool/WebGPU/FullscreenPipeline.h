#pragma once
#include <WebGPU/webgpu.hpp>
#include "Cool/WebGPU/BindGroup.h"
#include "Cool/WebGPU/Buffer.h"
#include "Cool/WebGPU/WGPUUnique.h"

namespace Cool {

// TODO(WebGPU) Turn this into a compute shader? removes the need for the dummy vao. Problem: some effects need ddx() and ddy()
class FullscreenPipeline : public WGPUUnique<wgpu::RenderPipeline> {
public:
    explicit FullscreenPipeline(std::string_view wgsl_fragment_shader_source_code);

    void set_uniforms() const;
    void draw(wgpu::RenderPassEncoder) const;

private:
    Buffer    _uniforms_buffer;
    BindGroup _bind_group;
};

} // namespace Cool
