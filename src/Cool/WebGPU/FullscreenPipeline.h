#pragma once
#include <webgpu/webgpu.hpp>
#include "Cool/WebGPU/WGPUUnique.h"

namespace Cool {

// TODO(WebGPU) Turn this into a compute shader? removes the need for the dummy vao. Problem: some effects need ddx() and ddy()
class FullscreenPipeline : public WGPUUnique<wgpu::RenderPipeline> {
public:
    explicit FullscreenPipeline(std::string_view wgsl_fragment_shader_source_code);

private:
};

} // namespace Cool
