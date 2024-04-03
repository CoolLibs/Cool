#pragma once
#include <webgpu/webgpu.hpp>

namespace Cool {

struct WebGPUContext {
    // TODO(WebGPU) Destroy the objects
    // TODO(WebGPU) Remove all the unused objects
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
    wgpu::RenderPassEncoder mainRenderPass{nullptr}; // TODO(WebGPU) Does it belong here?

    wgpu::TextureFormat       swapChainFormat    = wgpu::TextureFormat::Undefined;
    wgpu::TextureFormat       depthTextureFormat = wgpu::TextureFormat::Depth24Plus;
    wgpu::SwapChainDescriptor swapChainDesc;

    void check_for_swapchain_rebuild();
    void on_window_size_changed(int width, int height);

private:
    void buildSwapChain(int width, int height);
    void buildDepthBuffer(int width, int height);

public:
    int _swap_chain_width{};
    int _swap_chain_height{};
};

auto webgpu_context() -> WebGPUContext&;

} // namespace Cool