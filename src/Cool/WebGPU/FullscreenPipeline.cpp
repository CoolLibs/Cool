#include "FullscreenPipeline.h"
#include "BindGroup.hpp"
#include "BindGroupLayout.hpp"
#include "Cool/Gpu/WebGPUContext.h"
#include "ShaderModule.h"
#include "Texture.h"
#include "webgpu/webgpu.hpp"

namespace Cool {

static auto make_uniforms_buffer_descriptor(uint64_t size) -> wgpu::BufferDescriptor
{
    wgpu::BufferDescriptor bufferDesc{};
    bufferDesc.size             = size;
    bufferDesc.usage            = wgpu::BufferUsage::CopyDst | wgpu::BufferUsage::Uniform;
    bufferDesc.mappedAtCreation = false;
    return bufferDesc;
}

FullscreenPipeline::FullscreenPipeline(FullscreenPipeline_CreationArgs const& args)
    : _uniforms_buffer_fragment_shader{make_uniforms_buffer_descriptor(300 * sizeof(float))} // TODO(WebGPU) Don't hardcode the 300
    , _uniforms_buffer_vertex_shader{make_uniforms_buffer_descriptor(sizeof(float))}
    , _has_extra_bind_group{args.extra_bind_group_layout != nullptr}
{
    // Create bind group layout
    std::vector<wgpu::BindGroupLayoutEntry> entries(2, wgpu::Default);

    entries[0].binding               = 0; // The binding index as used in the @binding attribute in the shader
    entries[0].visibility            = wgpu::ShaderStage::Fragment;
    entries[0].buffer.type           = wgpu::BufferBindingType::Uniform;
    entries[0].buffer.minBindingSize = 300 * sizeof(float); // TODO(WebGPU) Don't hardcode the 300

    entries[1].binding               = 1;
    entries[1].visibility            = wgpu::ShaderStage::Vertex;
    entries[1].buffer.type           = wgpu::BufferBindingType::Uniform;
    entries[1].buffer.minBindingSize = sizeof(float);

    wgpu::BindGroupLayoutDescriptor bindGroupLayoutDesc{};
    bindGroupLayoutDesc.entryCount = entries.size();
    bindGroupLayoutDesc.entries    = entries.data();

    BindGroupLayout const bind_group_layout{bindGroupLayoutDesc};

    wgpu::RenderPipelineDescriptor pipelineDesc;

    // Vertex fetch
    // (We don't use any input buffer so far)
    pipelineDesc.vertex.bufferCount = 0;
    pipelineDesc.vertex.buffers     = nullptr;

    // Vertex shader
    ShaderModule vertex_shader{{
        .label = "[FullscreenPipeline] Vertex Shader",
        .code  = R"(
@group(0) @binding(1) var<uniform> _aspect_ratio: f32;

struct VertexOutput {
    @builtin(position) position: vec4f,
    @location(0) uv: vec2f,
};
@vertex
fn main(@builtin(vertex_index) in_vertex_index: u32) -> VertexOutput {
    var /*TODO(WebGPU) make this a const once wgpu-native supports it*/ vertices = array(vec2f(-1., -1.), vec2f(3., -1.), vec2f(-1., 3.));
    var out: VertexOutput;
    out.position = vec4f(vertices[in_vertex_index], 0., 1.);
    out.uv = vertices[in_vertex_index]; // -1 to 1
    out.uv.x *= _aspect_ratio;
    out.uv.y *= -1.; // We want y-axis going up
    return out;
})",
    }};
    pipelineDesc.vertex.module        = vertex_shader;
    pipelineDesc.vertex.entryPoint    = "main";
    pipelineDesc.vertex.constantCount = 0; // TODO(WebGPU) Pass _aspect_ratio as a constant rather than uniform?
    pipelineDesc.vertex.constants     = nullptr;

    // Primitive assembly and rasterization
    // Each sequence of 3 vertices is considered as a triangle
    pipelineDesc.primitive.topology = wgpu::PrimitiveTopology::TriangleList;
    // We'll see later how to specify the order in which vertices should be
    // connected. When not specified, vertices are considered sequentially.
    pipelineDesc.primitive.stripIndexFormat = wgpu::IndexFormat::Undefined;
    // The face orientation is defined by assuming that when looking
    // from the front of the face, its corner vertices are enumerated
    // in the counter-clockwise (CCW) order.
    pipelineDesc.primitive.frontFace = wgpu::FrontFace::CCW;
    // But the face orientation does not matter much because we do not
    // cull (i.e. "hide") the faces pointing away from us (which is often
    // used for optimization).
    pipelineDesc.primitive.cullMode = wgpu::CullMode::None;

    // Fragment shader
    ShaderModule        fragment_shader{args.fragment_shader_module_creation_args};
    wgpu::FragmentState fragmentState;
    pipelineDesc.fragment       = &fragmentState;
    fragmentState.module        = fragment_shader;
    fragmentState.entryPoint    = "main";
    fragmentState.constantCount = 0;
    fragmentState.constants     = nullptr;

    // Configure blend state
    wgpu::BlendState blendState;
    // Usual alpha blending, using premultiplied alpha:
    blendState.color.srcFactor = wgpu::BlendFactor::One;
    blendState.color.dstFactor = wgpu::BlendFactor::OneMinusSrcAlpha;
    blendState.color.operation = wgpu::BlendOperation::Add;
    //
    blendState.alpha.srcFactor = wgpu::BlendFactor::One;
    blendState.alpha.dstFactor = wgpu::BlendFactor::OneMinusSrcAlpha;
    blendState.alpha.operation = wgpu::BlendOperation::Add;

    wgpu::ColorTargetState colorTarget;
    colorTarget.format    = wgpu::TextureFormat::RGBA8Unorm /* Cool::webgpu_context().swapChainFormat */; // TODO(WebGPU) share state description between RenderPipeline and RenderTarget, and allow users to customize it
    colorTarget.blend     = &blendState;
    colorTarget.writeMask = wgpu::ColorWriteMask::All; // We could write to only some of the color channels.

    // We have only one target because our render pass has only one output color
    // attachment.
    fragmentState.targetCount = 1;
    fragmentState.targets     = &colorTarget;

    // We setup a depth buffer state for the render pipeline
    // wgpu::DepthStencilState depthStencilState = wgpu::Default;
    // // Keep a fragment only if its depth is lower than the previously blended one
    // depthStencilState.depthCompare = wgpu::CompareFunction::Less;
    // // Each time a fragment is blended into the target, we update the value of the Z-buffer
    // depthStencilState.depthWriteEnabled = true;
    // // Store the format in a variable as later parts of the code depend on it
    // wgpu::TextureFormat depthTextureFormat = wgpu::TextureFormat::Depth24Plus;
    // depthStencilState.format               = depthTextureFormat;
    // // Deactivate the stencil alltogether
    // depthStencilState.stencilReadMask  = 0;
    // depthStencilState.stencilWriteMask = 0;

    pipelineDesc.depthStencil = nullptr; // &depthStencilState;

    // Multi-sampling
    // Samples per pixel
    pipelineDesc.multisample.count = 1;
    // Default value for the mask, meaning "all bits on"
    pipelineDesc.multisample.mask = ~0u;
    // Default value as well (irrelevant for count = 1 anyways)
    pipelineDesc.multisample.alphaToCoverageEnabled = false;

    // Pipeline layout

    // Create the pipeline layout
    if (args.extra_bind_group_layout)
    {
        wgpu::PipelineLayoutDescriptor layoutDesc{};
        auto const                     layouts = std::vector<WGPUBindGroupLayout>{bind_group_layout, *args.extra_bind_group_layout};
        layoutDesc.bindGroupLayoutCount        = layouts.size();
        layoutDesc.bindGroupLayouts            = layouts.data();
        pipelineDesc.layout                    = webgpu_context().device.createPipelineLayout(layoutDesc);
    }
    else
    {
        wgpu::PipelineLayoutDescriptor layoutDesc{};
        layoutDesc.bindGroupLayoutCount = 1;
        layoutDesc.bindGroupLayouts     = (WGPUBindGroupLayout*)&bind_group_layout;
        pipelineDesc.layout             = webgpu_context().device.createPipelineLayout(layoutDesc);
    }

    set_handle(Cool::webgpu_context().device.createRenderPipeline(pipelineDesc));

    // Create a bind group
    std::vector<wgpu::BindGroupEntry> bindings(2, wgpu::Default);
    bindings[0].binding = entries[0].binding;               // The index of the binding (the entries in bindGroupDesc can be in any order)
    bindings[0].buffer  = _uniforms_buffer_fragment_shader; // The buffer it is actually bound to
    bindings[0].offset  = 0;                                // We can specify an offset within the buffer, so that a single buffer can hold multiple uniform blocks.
    bindings[0].size    = entries[0].buffer.minBindingSize; // And we specify again the size of the buffer.

    bindings[1].binding = entries[1].binding;
    bindings[1].buffer  = _uniforms_buffer_vertex_shader;
    bindings[1].offset  = 0;
    bindings[1].size    = entries[1].buffer.minBindingSize;

    // A bind group contains one or multiple bindings
    wgpu::BindGroupDescriptor bindGroupDesc{};
    bindGroupDesc.layout = bind_group_layout;
    // There must be as many bindings as declared in the layout!
    bindGroupDesc.entryCount = bindGroupLayoutDesc.entryCount;
    bindGroupDesc.entries    = bindings.data();
    _bind_group              = BindGroup{bindGroupDesc};
}

void FullscreenPipeline::set_aspect_ratio_uniform(float aspect_ratio) const
{
    webgpu_context().queue.writeBuffer(_uniforms_buffer_vertex_shader, 0, &aspect_ratio, sizeof(aspect_ratio));
}

void FullscreenPipeline::draw(wgpu::RenderPassEncoder& render_pass, BindGroup const* extra_bind_group) const
{
    render_pass.setPipeline(*this);
    render_pass.setBindGroup(0, _bind_group, 0, nullptr);
    // TODO(WebGPU) Cleanup :
    // if (_has_extra_bind_group)
    // {
    //     assert(extra_bind_group != nullptr);
    //     render_pass.setBindGroup(1, *extra_bind_group, 0, nullptr);
    // }
    // else
    // {
    //     assert(extra_bind_group == nullptr);
    // }
    render_pass.draw(3, 1, 0, 0);
}

} // namespace Cool
