#pragma once
#include <webgpu/webgpu.hpp>

namespace Cool {

struct WebGPUContext {
    wgpu::Instance          instance{nullptr};
    wgpu::Surface           surface{nullptr};
    wgpu::Adapter           adapter{nullptr};
    wgpu::Device            device{nullptr};
    wgpu::Queue             queue{nullptr};
    wgpu::SwapChain         swapChain{nullptr};
    wgpu::ShaderModule      shaderModule{nullptr};
    wgpu::RenderPipeline    pipeline{nullptr};
    wgpu::Texture           depthTexture{nullptr};
    wgpu::TextureView       depthTextureView{nullptr};
    wgpu::Sampler           sampler{nullptr};
    wgpu::TextureView       textureView{nullptr};
    wgpu::Texture           texture{nullptr};
    wgpu::Buffer            vertexBuffer{nullptr};
    wgpu::Buffer            uniformBuffer{nullptr};
    wgpu::BindGroup         bindGroup{nullptr};
    wgpu::RenderPassEncoder mainRenderPass{nullptr};

    wgpu::TextureFormat       swapChainFormat    = wgpu::TextureFormat::Undefined;
    wgpu::TextureFormat       depthTextureFormat = wgpu::TextureFormat::Depth24Plus;
    wgpu::SwapChainDescriptor swapChainDesc;

    // TODO(WebGPU) Destroy the objects
};

auto webgpu_context() -> WebGPUContext&;

} // namespace Cool