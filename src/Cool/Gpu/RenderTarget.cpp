#include "RenderTarget.h"
#include "Cool/Gpu/WebGPUContext.h"
#include "Cool/WebGPU/ShaderModule.h"
#include "Cool/WebGPU/Texture.h"
#include "webgpu/webgpu.hpp"

namespace Cool {

void RenderTarget::render(std::function<void(wgpu::RenderPassEncoder render_pass)> const& render_fn)
{
    resize_if_necessary();
    assert(_texture.handle() != nullptr);

    wgpu::CommandEncoderDescriptor commandEncoderDesc;
    commandEncoderDesc.label     = "Command Encoder";
    wgpu::CommandEncoder encoder = webgpu_context().device.createCommandEncoder(commandEncoderDesc);

    wgpu::RenderPassDescriptor renderPassDesc{};
    TextureView const&         nextTexture{_texture.entire_texture_view()};

    wgpu::RenderPassColorAttachment renderPassColorAttachment{};
    renderPassColorAttachment.view          = nextTexture;
    renderPassColorAttachment.resolveTarget = nullptr;
    renderPassColorAttachment.loadOp        = wgpu::LoadOp::Clear; // TODO(WebGPU) bool to choose if we want to clear or not
    renderPassColorAttachment.storeOp       = wgpu::StoreOp::Store;
    renderPassColorAttachment.clearValue    = wgpu::Color{0., 0., 0., 0.};
    renderPassDesc.colorAttachmentCount     = 1;
    renderPassDesc.colorAttachments         = &renderPassColorAttachment;

    // TODO(WebGPU) bool to choose if we want depth buffer or not
    //     wgpu::RenderPassDepthStencilAttachment depthStencilAttachment;
    //     depthStencilAttachment.view              = webgpu_context().depthTextureView;
    //     depthStencilAttachment.depthClearValue   = 1.0f;
    //     depthStencilAttachment.depthLoadOp       = wgpu::LoadOp::Clear;
    //     depthStencilAttachment.depthStoreOp      = wgpu::StoreOp::Store;
    //     depthStencilAttachment.depthReadOnly     = false;
    //     depthStencilAttachment.stencilClearValue = 0;
    // #ifdef WEBGPU_BACKEND_WGPU
    //     depthStencilAttachment.stencilLoadOp  = wgpu::LoadOp::Clear;
    //     depthStencilAttachment.stencilStoreOp = wgpu::StoreOp::Store;
    // #else
    //     depthStencilAttachment.stencilLoadOp  = wgpu::LoadOp::Undefined;
    //     depthStencilAttachment.stencilStoreOp = wgpu::StoreOp::Undefined;
    // #endif
    //     depthStencilAttachment.stencilReadOnly = true;

    renderPassDesc.depthStencilAttachment = nullptr; //&depthStencilAttachment;

    renderPassDesc.timestampWriteCount = 0;
    renderPassDesc.timestampWrites     = nullptr;

    wgpu::RenderPassEncoder render_pass = encoder.beginRenderPass(renderPassDesc);

    render_fn(render_pass); // TODO(WebGPU) What happens if this function throws ?

    // Render was successful, _texture has changed, so _texture_straight_alpha will need to update
    _texture_straight_alpha.reset();

    render_pass.end();
    render_pass.release();

    wgpu::CommandBufferDescriptor cmdBufferDescriptor{};
    cmdBufferDescriptor.label   = "Command buffer";
    wgpu::CommandBuffer command = encoder.finish(cmdBufferDescriptor);
    encoder.release();
    webgpu_context().queue.submit(command);
    command.release();
}

auto RenderTarget::texture_straight_alpha() const -> Texture const&
{
    // TODO(WebGPU)
    // if (_texture.alpha_space == AlphaSpace::Straight())
    //     return _texture;

    if (!_texture_straight_alpha.has_value())
        make_texture_straight_alpha();
    assert(_texture_straight_alpha.has_value());
    return *_texture_straight_alpha;
}

void RenderTarget::make_texture_straight_alpha() const
{
    wgpu::TextureDescriptor textureDesc;
    textureDesc.dimension     = wgpu::TextureDimension::_2D;
    textureDesc.size          = {current_size().width(), current_size().height(), 1};
    textureDesc.mipLevelCount = 1;
    textureDesc.sampleCount   = 1;
    textureDesc.format        = wgpu::TextureFormat::RGBA8Unorm;

    textureDesc.usage = wgpu::TextureUsage::TextureBinding
                        | wgpu::TextureUsage::StorageBinding;
    textureDesc.viewFormatCount = 0;
    textureDesc.viewFormats     = nullptr;

    _texture_straight_alpha = Texture{textureDesc};

    // Initialize a command encoder
    wgpu::CommandEncoderDescriptor encoderDesc = wgpu::Default;
    wgpu::CommandEncoder           encoder     = webgpu_context().device.createCommandEncoder(encoderDesc);

    // Create and use compute pass here!
    // Create compute pass
    wgpu::ComputePassDescriptor computePassDesc;
    computePassDesc.timestampWriteCount  = 0;
    computePassDesc.timestampWrites      = nullptr;
    wgpu::ComputePassEncoder computePass = encoder.beginComputePass(computePassDesc);

    // In initComputePipeline():

    // Load compute shader
    ShaderModule computeShaderModule{
        R"wgsl(
@group(0) @binding(0) var in_tex_premultiplied: texture_2d<f32>;
@group(0) @binding(1) var out_tex_straight: texture_storage_2d<rgba8unorm,write>;

@compute @workgroup_size(8, 8)
fn main(@builtin(global_invocation_id) id: vec3<u32>) {
    let color = textureLoad(in_tex_premultiplied, id.xy, 0);
    textureStore(out_tex_straight, id.xy, 
        vec4(
            select(color.rgb / color.a, vec3(0.), abs(color.a) < 0.0000001f),
            color.a
        )
    );
}
)wgsl",
    };

    // Create bind group layout
    std::vector<wgpu::BindGroupLayoutEntry> bindings(2, wgpu::Default);

    // Input texture
    bindings[0].binding               = 0;
    bindings[0].texture.sampleType    = wgpu::TextureSampleType::Float;
    bindings[0].texture.viewDimension = wgpu::TextureViewDimension::_2D;
    bindings[0].visibility            = wgpu::ShaderStage::Compute;

    // Output texture
    bindings[1].binding                      = 1;
    bindings[1].storageTexture.access        = wgpu::StorageTextureAccess::WriteOnly;
    bindings[1].storageTexture.format        = wgpu::TextureFormat::RGBA8Unorm;
    bindings[1].storageTexture.viewDimension = wgpu::TextureViewDimension::_2D;
    bindings[1].visibility                   = wgpu::ShaderStage::Compute;

    wgpu::BindGroupLayoutDescriptor bindGroupLayoutDesc;
    bindGroupLayoutDesc.entryCount          = (uint32_t)bindings.size();
    bindGroupLayoutDesc.entries             = bindings.data();
    wgpu::BindGroupLayout m_bindGroupLayout = webgpu_context().device.createBindGroupLayout(bindGroupLayoutDesc);

    // Create compute bind group
    std::vector<wgpu::BindGroupEntry> entries(2, wgpu::Default);

    // Input texture
    entries[0].binding     = 0;
    entries[0].textureView = _texture.entire_texture_view();

    // Output texture
    entries[1].binding     = 1;
    entries[1].textureView = _texture_straight_alpha->entire_texture_view();

    wgpu::BindGroupDescriptor bindGroupDesc;
    bindGroupDesc.layout        = m_bindGroupLayout;
    bindGroupDesc.entryCount    = (uint32_t)entries.size();
    bindGroupDesc.entries       = (WGPUBindGroupEntry*)entries.data();
    wgpu::BindGroup m_bindGroup = webgpu_context().device.createBindGroup(bindGroupDesc);
    // Create compute pipeline
    wgpu::ComputePipelineDescriptor computePipelineDesc = wgpu::Default;
    computePipelineDesc.label                           = "[RenderTarget] Convert premultiplied to straight alpha";
    computePipelineDesc.compute.entryPoint              = "main";
    computePipelineDesc.compute.module                  = computeShaderModule;

    // Create compute pipeline layout
    wgpu::PipelineLayoutDescriptor pipelineLayoutDesc;
    pipelineLayoutDesc.bindGroupLayoutCount = 1;
    pipelineLayoutDesc.bindGroupLayouts     = (WGPUBindGroupLayout*)&m_bindGroupLayout;
    wgpu::PipelineLayout m_pipelineLayout   = webgpu_context().device.createPipelineLayout(pipelineLayoutDesc);
    computePipelineDesc.layout              = m_pipelineLayout;

    wgpu::ComputePipeline computePipeline = webgpu_context().device.createComputePipeline(computePipelineDesc);

    // Use compute pass
    computePass.setPipeline(computePipeline);
    // Set the bind group:
    computePass.setBindGroup(0, m_bindGroup, 0, nullptr);
    {
        uint32_t invocationCountX    = _texture.size().width();
        uint32_t invocationCountY    = _texture.size().height();
        uint32_t workgroupSizePerDim = 8;
        // This ceils invocationCountX / workgroupSizePerDim
        uint32_t workgroupCountX = (invocationCountX + workgroupSizePerDim - 1) / workgroupSizePerDim;
        uint32_t workgroupCountY = (invocationCountY + workgroupSizePerDim - 1) / workgroupSizePerDim;
        computePass.dispatchWorkgroups(workgroupCountX, workgroupCountY, 1);
    }

    // Finalize compute pass
    computePass.end();

// Clean up
#if !defined(WEBGPU_BACKEND_WGPU)
    computePass.release();
#endif

    // Encode and submit the GPU commands
    wgpu::CommandBuffer commands = encoder.finish(wgpu::CommandBufferDescriptor{});
    webgpu_context().queue.submit(commands);

    // Clean up
#if !defined(WEBGPU_BACKEND_WGPU)
    commands.release();
    encoder.release();
#endif
}

void RenderTarget::set_size(img::Size size)
{
    _desired_size = size;
}

void RenderTarget::set_size_immediately(img::Size size)
{
    set_size(size);
    resize_if_necessary();
}

auto RenderTarget::needs_resizing() const -> bool
{
    return _texture.handle() == nullptr || _texture.size() != desired_size();
}

void RenderTarget::resize_if_necessary()
{
    if (!needs_resizing())
        return;

    wgpu::TextureDescriptor textureDesc;
    textureDesc.dimension       = wgpu::TextureDimension::_2D;
    textureDesc.size            = {_desired_size.width(), _desired_size.height(), 1};
    textureDesc.mipLevelCount   = 1;
    textureDesc.sampleCount     = 1;
    textureDesc.format          = wgpu::TextureFormat::RGBA8Unorm;
    textureDesc.usage           = wgpu::TextureUsage::TextureBinding | wgpu::TextureUsage::RenderAttachment /*  | TextureUsage::CopyDst */;
    textureDesc.viewFormatCount = 0;
    textureDesc.viewFormats     = nullptr;

    _texture = Texture{textureDesc};
    _texture_straight_alpha.reset();
}

} // namespace Cool