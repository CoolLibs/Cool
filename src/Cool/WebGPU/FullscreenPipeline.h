#pragma once
#include <webgpu/webgpu.hpp>
#include "Cool/WebGPU/BindGroup.h"
#include "Cool/WebGPU/Buffer.h"
#include "Cool/WebGPU/ShaderModule.h"
#include "Cool/WebGPU/WGPUUnique.h"

namespace Cool {

// TODO(WebGPU) Turn this into a compute shader? removes the need for the dummy vao. Problem: some effects need ddx() and ddy()
class FullscreenPipeline : public WGPUUnique<wgpu::RenderPipeline> {
public:
    explicit FullscreenPipeline(ShaderModule_CreationArgs const& args);

    void set_uniforms(float aspect_ratio) const;
    void draw(wgpu::RenderPassEncoder) const;

private:
    Buffer    _uniforms_buffer;
    BindGroup _bind_group;
};

} // namespace Cool
