#include "ComputePipeline.h"
#include "Cool/Gpu/WebGPUContext.h"
#include "Cool/String/String.h"
#include "Cool/WebGPU/ShaderModule.h"
#include "webgpu/webgpu.hpp"

namespace Cool {

// static auto make_uniforms_buffer_descriptor() -> wgpu::BufferDescriptor
// {
//     wgpu::BufferDescriptor bufferDesc{};
//     bufferDesc.size             = sizeof(float);
//     bufferDesc.usage            = wgpu::BufferUsage::CopyDst | wgpu::BufferUsage::Uniform;
//     bufferDesc.mappedAtCreation = false;
//     return bufferDesc;
// }

ComputePipeline::ComputePipeline(ComputePipeline_CreationArgs const& args)
    // : _uniforms_buffer(make_uniforms_buffer_descriptor())
    : _workgroup_size{args.workgroup_size}
    , _bind_group_layout{args.bind_group_layout.build()}
{
    assert(_workgroup_size.x > 0);
    assert(_workgroup_size.y > 0);
    assert(_workgroup_size.z > 0);

    auto const compute_shader = ShaderModule{{
        .label = args.label,
        .code  = String::replace_all_words(
            std::string{args.wgsl_compute_shader_code},
            "@compute",
            fmt::format("@compute @workgroup_size({}, {}, {})", _workgroup_size.x, _workgroup_size.y, _workgroup_size.z)
        ),
    }};

    wgpu::ComputePipelineDescriptor computePipelineDesc = wgpu::Default;
    computePipelineDesc.label                           = args.label.data();
    computePipelineDesc.compute.entryPoint              = args.entry_point.data();
    computePipelineDesc.compute.module                  = compute_shader;

    wgpu::PipelineLayoutDescriptor pipeline_layout_descriptor;
    pipeline_layout_descriptor.bindGroupLayoutCount = 1;
    pipeline_layout_descriptor.bindGroupLayouts     = reinterpret_cast<WGPUBindGroupLayout*>(&_bind_group_layout); // NOLINT(*reinterpret-cast)

    wgpu::PipelineLayout pipeline_layout = webgpu_context().device.createPipelineLayout(pipeline_layout_descriptor);
    computePipelineDesc.layout           = pipeline_layout;

    set_handle(webgpu_context().device.createComputePipeline(computePipelineDesc));
    pipeline_layout.release();
}

// void ComputePipeline::set_uniforms(float aspect_ratio) const
// {
//     webgpu_context().queue.writeBuffer(_uniforms_buffer, 0, &aspect_ratio, sizeof(float));
// }

static void set(wgpu::BindGroupEntry& entry, Buffer const& buffer)
{
    entry.buffer = buffer;
    entry.offset = 0;
    entry.size   = buffer.handle().getSize();
}
static void set(wgpu::BindGroupEntry& entry, TextureView const& texture)
{
    entry.textureView = texture;
}

void ComputePipeline::compute(ComputePipeline_ComputeArgs const& args) const
{
    auto compute_pass = [&]() -> wgpu::ComputePassEncoder // IIFE
    {
        auto compute_pass_desc                = wgpu::ComputePassDescriptor{};
        compute_pass_desc.timestampWriteCount = 0;
        compute_pass_desc.timestampWrites     = nullptr;
        return args.encoder.value_or(webgpu_context().encoder).beginComputePass(compute_pass_desc);
    }();

    compute_pass.setPipeline(handle());

    // Create compute bind group
    // TODO(WebGPU) Move to BindGroup file
    std::vector<wgpu::BindGroupEntry> entries(args.bind_group.size(), wgpu::Default);
    for (size_t i = 0; i < entries.size(); ++i)
    {
        entries[i].binding = static_cast<uint32_t>(i);
        std::visit([&](auto&& truc) { set(entries[i], truc.get()); }, args.bind_group[i]);
    }
    wgpu::BindGroupDescriptor bindGroupDesc;
    bindGroupDesc.layout       = _bind_group_layout;
    bindGroupDesc.entryCount   = static_cast<uint32_t>(entries.size());
    bindGroupDesc.entries      = static_cast<WGPUBindGroupEntry*>(entries.data());
    wgpu::BindGroup bind_group = webgpu_context().device.createBindGroup(bindGroupDesc);

    compute_pass.setBindGroup(0, bind_group, 0, nullptr);

    compute_pass.dispatchWorkgroups(
        (args.invocation_count_x + _workgroup_size.x - 1) / _workgroup_size.x,
        (args.invocation_count_y + _workgroup_size.y - 1) / _workgroup_size.y,
        (args.invocation_count_z + _workgroup_size.z - 1) / _workgroup_size.z
    );
    bind_group.release();
    compute_pass.end();
    compute_pass.release();
}

} // namespace Cool
