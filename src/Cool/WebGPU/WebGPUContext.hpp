#pragma once
#include <img/src/Size.h>
#include <webgpu/webgpu.hpp>

namespace Cool {

class WebGPUContext {
public:
    // TODO(WebGPU) Destroy the objects
    // TODO(WebGPU) Remove all the unused objects
    wgpu::Instance       instance{nullptr};
    wgpu::Surface        surface{nullptr};
    wgpu::Adapter        adapter{nullptr};
    wgpu::Device         device{nullptr};
    wgpu::Queue          queue{nullptr};
    wgpu::CommandEncoder encoder{nullptr};
    wgpu::TextureFormat  surface_format{wgpu::TextureFormat::Undefined};

    void check_for_swapchain_rebuild(img::Size swapchain_size);

private:
    void build_swapchain(img::Size swapchain_size);

private:
    img::Size _swapchain_size{};
};

auto webgpu_context() -> WebGPUContext&;

} // namespace Cool