#include "WebGPUContext.h"
#include "Cool/Backend/BackendContext.h"

namespace Cool {

auto webgpu_context() -> WebGPUContext&
{
    return backend_context()->webgpu_context();
}

void WebGPUContext::buildSwapChain(int width, int height)
{
    // Destroy previously allocated swap chain
    if (swapChain != nullptr)
        swapChain.release();

#ifdef WEBGPU_BACKEND_WGPU
    swapChainFormat = surface.getPreferredFormat(adapter);
#else
    swapChainFormat = wgpu::TextureFormat::BGRA8Unorm; // Dawn does not support getPreferredFormat() yet, (and I don't know what happens when building for the web)
#endif
    swapChainDesc.width       = static_cast<uint32_t>(width);
    swapChainDesc.height      = static_cast<uint32_t>(height);
    swapChainDesc.usage       = wgpu::TextureUsage::RenderAttachment;
    swapChainDesc.format      = swapChainFormat;
    swapChainDesc.presentMode = wgpu::PresentMode::Fifo;
    swapChain                 = device.createSwapChain(surface, swapChainDesc);
}

void WebGPUContext::buildDepthBuffer(int width, int height)
{
    // Destroy previously allocated texture
    if (depthTexture != nullptr)
    {
        depthTextureView.release();
        depthTexture.destroy();
        depthTexture.release();
    }

    // Create the depth texture
    wgpu::TextureDescriptor depthTextureDesc;
    depthTextureDesc.dimension       = wgpu::TextureDimension::_2D;
    depthTextureDesc.format          = depthTextureFormat;
    depthTextureDesc.mipLevelCount   = 1;
    depthTextureDesc.sampleCount     = 1;
    depthTextureDesc.size            = {static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1};
    depthTextureDesc.usage           = wgpu::TextureUsage::RenderAttachment;
    depthTextureDesc.viewFormatCount = 1;
    depthTextureDesc.viewFormats     = (WGPUTextureFormat*)&depthTextureFormat;
    depthTexture                     = device.createTexture(depthTextureDesc);

    // Create the view of the depth texture manipulated by the rasterizer
    wgpu::TextureViewDescriptor depthTextureViewDesc;
    depthTextureViewDesc.aspect          = wgpu::TextureAspect::DepthOnly;
    depthTextureViewDesc.baseArrayLayer  = 0;
    depthTextureViewDesc.arrayLayerCount = 1;
    depthTextureViewDesc.baseMipLevel    = 0;
    depthTextureViewDesc.mipLevelCount   = 1;
    depthTextureViewDesc.dimension       = wgpu::TextureViewDimension::_2D;
    depthTextureViewDesc.format          = depthTextureFormat;
    depthTextureView                     = depthTexture.createView(depthTextureViewDesc);
}

void WebGPUContext::check_for_swapchain_rebuild()
{
    int width, height; // NOLINT(*isolate-declaration, *init-variables)
    glfwGetFramebufferSize(window().glfw(), &width, &height);
    if (width != _swap_chain_width || height != _swap_chain_height)
        on_window_size_changed(width, height);
}

void WebGPUContext::on_window_size_changed(int width, int height)
{
    buildSwapChain(width, height);
    buildDepthBuffer(width, height); // TODO(WebGPU) Is this needed for Coollab?
    _swap_chain_width  = width;
    _swap_chain_height = height;
}

} // namespace Cool