#include "WebGPUContext.hpp"
#include <webgpu/webgpu.hpp>
#include "Cool/Backend/BackendContext.h"

namespace Cool {

auto webgpu_context() -> WebGPUContext&
{
    return backend_context()->webgpu_context();
}

void WebGPUContext::build_swapchain(img::Size size)
{
    // TODO(WebGPU) Do we need to unconfigure the surface before configuring it again ?
    // Destroy previously allocated swap chain
    // if (swapChain != nullptr)
    //     swapChain.release();
    auto config            = wgpu::SurfaceConfiguration{};
    config.width           = size.width();
    config.height          = size.height();
    config.format          = surface_format;
    config.viewFormatCount = 0;
    config.viewFormats     = nullptr;
    config.usage           = wgpu::TextureUsage::RenderAttachment;
    config.device          = device;
    config.presentMode     = wgpu::PresentMode::Fifo;
    config.alphaMode       = wgpu::CompositeAlphaMode::Auto; // TODO(WebGPU) If we switch to rendering our images in premultiplied mode, we need to change that (although we don't really care about transparent windows)
    surface.configure(config);
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