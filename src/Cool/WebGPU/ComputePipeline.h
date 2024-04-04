#pragma once
#include <webgpu/webgpu.hpp>
#include "Cool/WebGPU/TextureView.h"
#include "Cool/WebGPU/WGPUUnique.h"

namespace Cool {

struct ComputePipeline_CreationArgs {
    std::string_view                label{"Compute Pass"};
    wgpu::BindGroupLayoutDescriptor bind_group_layout_desc{};
    glm::uvec3                      workgroup_size{32, 1, 1}; // NB: prefer {32, 1, 1} for 1D operations, and {8, 8, 1} for 2D operations. See https://eliemichel.github.io/LearnWebGPU/basic-compute/image-processing/mipmap-generation.html#dispatch
    std::string_view                entry_point{"main"};
    std::string_view                wgsl_compute_shader_code{};
};

struct ComputePipeline_ComputeArgs {
    uint32_t invocation_count_x = 1;
    uint32_t invocation_count_y = 1;
    uint32_t invocation_count_z = 1;

    std::vector<std::reference_wrapper<TextureView const>> bind_group{}; // TODO(WebGPU) Allow buffers too
};

// TODO(WebGPU) Turn this into a compute shader? removes the need for the dummy vao. Problem: some effects need ddx() and ddy()
class ComputePipeline : public WGPUUnique<wgpu::ComputePipeline> {
public:
    ComputePipeline(ComputePipeline_CreationArgs const&); // NOLINT(*explicit*)

    // void set_uniforms(float aspect_ratio) const;
    void compute(ComputePipeline_ComputeArgs const&) const;

private:
    glm::uvec3            _workgroup_size{};
    wgpu::BindGroupLayout _bind_group_layout{nullptr};
    // Buffer    _uniforms_buffer;
};

} // namespace Cool
