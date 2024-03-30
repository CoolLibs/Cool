#include "FullscreenPipeline.h"
#include <webgpu/webgpu.hpp>
#include "Cool/Gpu/WebGPUContext.h"
#include "Cool/WebGPU/ShaderModule.h"

namespace Cool {

static auto make_fullscreen_pipeline(std::string_view wgsl_fragment_shader_source_code) -> wgpu::RenderPipeline
{
    wgpu::RenderPipelineDescriptor pipelineDesc;

    // Vertex fetch
    // (We don't use any input buffer so far)
    pipelineDesc.vertex.bufferCount = 0;
    pipelineDesc.vertex.buffers     = nullptr;

    // Vertex shader
    ShaderModule vertex_shader{R"(
@vertex
fn main(@builtin(vertex_index) in_vertex_index: u32) -> @builtin(position) vec4<f32> {
    const vertices = array(vec2f(-1., -1.), vec2f(3., -1.), vec2f(-1., 3.));
    return vec4f(vertices[in_vertex_index], 0., 1.);
    // _uv              = 0.5 * vertices[gl_VertexIndex] + vec2(0.5); //TODO(WebGPU)
})"};
    pipelineDesc.vertex.module        = vertex_shader.handle();
    pipelineDesc.vertex.entryPoint    = "main";
    pipelineDesc.vertex.constantCount = 0;
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
    ShaderModule        fragment_shader{wgsl_fragment_shader_source_code};
    wgpu::FragmentState fragmentState;
    pipelineDesc.fragment       = &fragmentState;
    fragmentState.module        = fragment_shader.handle();
    fragmentState.entryPoint    = "main";
    fragmentState.constantCount = 0;
    fragmentState.constants     = nullptr;

    // Configure blend state
    wgpu::BlendState blendState;
    // Usual alpha blending for the color:
    blendState.color.srcFactor = wgpu::BlendFactor::SrcAlpha;
    blendState.color.dstFactor = wgpu::BlendFactor::OneMinusSrcAlpha;
    blendState.color.operation = wgpu::BlendOperation::Add;
    // We leave the target alpha untouched:
    blendState.alpha.srcFactor = wgpu::BlendFactor::Zero;
    blendState.alpha.dstFactor = wgpu::BlendFactor::One;
    blendState.alpha.operation = wgpu::BlendOperation::Add;

    wgpu::ColorTargetState colorTarget;
    colorTarget.format    = Cool::webgpu_context().swapChainFormat;
    colorTarget.blend     = &blendState;
    colorTarget.writeMask = wgpu::ColorWriteMask::All; // We could write to only some of the color channels.

    // We have only one target because our render pass has only one output color
    // attachment.
    fragmentState.targetCount = 1;
    fragmentState.targets     = &colorTarget;

    // We setup a depth buffer state for the render pipeline
    wgpu::DepthStencilState depthStencilState = wgpu::Default;
    // Keep a fragment only if its depth is lower than the previously blended one
    depthStencilState.depthCompare = wgpu::CompareFunction::Less;
    // Each time a fragment is blended into the target, we update the value of the Z-buffer
    depthStencilState.depthWriteEnabled = true;
    // Store the format in a variable as later parts of the code depend on it
    wgpu::TextureFormat depthTextureFormat = wgpu::TextureFormat::Depth24Plus;
    depthStencilState.format               = depthTextureFormat;
    // Deactivate the stencil alltogether
    depthStencilState.stencilReadMask  = 0;
    depthStencilState.stencilWriteMask = 0;

    pipelineDesc.depthStencil = &depthStencilState;

    // Multi-sampling
    // Samples per pixel
    pipelineDesc.multisample.count = 1;
    // Default value for the mask, meaning "all bits on"
    pipelineDesc.multisample.mask = ~0u;
    // Default value as well (irrelevant for count = 1 anyways)
    pipelineDesc.multisample.alphaToCoverageEnabled = false;

    // Pipeline layout
    pipelineDesc.layout = nullptr;

    return Cool::webgpu_context().device.createRenderPipeline(pipelineDesc);
}

FullscreenPipeline::FullscreenPipeline(std::string_view wgsl_fragment_shader_source_code)
    : WGPUUnique<wgpu::RenderPipeline>{make_fullscreen_pipeline(wgsl_fragment_shader_source_code)}
{}

} // namespace Cool
