#include "WebGPUContext.h"
#include "Cool/Backend/BackendContext.h"

namespace Cool {

auto webgpu_context() -> WebGPUContext&
{
    return backend_context()->webgpu_context();
}

void WebGPUContext::build_swapchain(img::Size size)
{
    // Destroy previously allocated swap chain
    if (swapChain != nullptr)
        swapChain.release();

#ifdef WEBGPU_BACKEND_WGPU
    swapChainFormat = surface.getPreferredFormat(adapter);
#else
    swapChainFormat = wgpu::TextureFormat::BGRA8Unorm; // Dawn does not support getPreferredFormat() yet, (and I don't know what happens when building for the web)
#endif
    swapChainDesc.width       = size.width();
    swapChainDesc.height      = size.height();
    swapChainDesc.usage       = wgpu::TextureUsage::RenderAttachment;
    swapChainDesc.format      = swapChainFormat;
    swapChainDesc.presentMode = wgpu::PresentMode::Fifo;
    swapChain                 = device.createSwapChain(surface, swapChainDesc);
}

void WebGPUContext::check_for_swapchain_rebuild(img::Size swapchain_size)
{
    if (swapchain_size == _swapchain_size)
        return;

    // static bool first = true;
    // if (!first)
    //     ImGui_ImplWGPU_InvalidateDeviceObjects();
    build_swapchain(swapchain_size);
    // if (!first)
    //     ImGui_ImplWGPU_CreateDeviceObjects();
    // first = false;
    _swapchain_size = swapchain_size;
}

} // namespace Cool