#pragma once
#include "Cool/WebGPU/WGPUUnique.h"
#include "webgpu/webgpu.hpp"

namespace Cool {

class Buffer : public WGPUUnique<wgpu::Buffer> {
public:
    explicit Buffer(wgpu::BufferDescriptor const&);

    auto size_in_bytes() const -> size_t { return _size_in_bytes; }

private:
    size_t _size_in_bytes{};
};

auto buffer_from_data(std::span<float const> data) -> Buffer; // TODO(WebGPU) support other types than float

} // namespace Cool