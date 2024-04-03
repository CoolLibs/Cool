#include "RenderTarget.h"
#include "Cool/Gpu/WebGPUContext.h"
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

    render_fn(render_pass);

    render_pass.end();
    render_pass.release();

    wgpu::CommandBufferDescriptor cmdBufferDescriptor{};
    cmdBufferDescriptor.label   = "Command buffer";
    wgpu::CommandBuffer command = encoder.finish(cmdBufferDescriptor);
    encoder.release();
    webgpu_context().queue.submit(command);
    command.release();
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
}

} // namespace Cool