#include "Buffer.h"
#include <webgpu/webgpu.hpp>
#include "Cool/WebGPU/WebGPUContext.hpp"

namespace Cool {

Buffer::Buffer(wgpu::BufferDescriptor const& desc)
    : WGPUUnique<wgpu::Buffer>{webgpu_context().device.createBuffer(desc)}
    , _size_in_bytes{desc.size}
{}

auto buffer_from_data(std::span<float const> data) -> Buffer
{
    auto desc             = wgpu::BufferDescriptor{};
    desc.size             = sizeof(float) * data.size();
    desc.usage            = wgpu::BufferUsage::Storage | wgpu::BufferUsage::CopyDst;
    desc.mappedAtCreation = false;
    auto buffer           = Buffer{desc};

    webgpu_context().queue.writeBuffer(buffer, 0, data.data(), desc.size);
    webgpu_context().queue.submit(0, nullptr);

    return buffer;
}

} // namespace Cool