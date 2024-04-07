#pragma once
#include "Cool/WebGPU/WGPUUnique.h"
#include "webgpu/webgpu.hpp"

namespace Cool {

class TextureView : public WGPUUnique<wgpu::TextureView> {
public:
    TextureView(wgpu::TextureView); // NOLINT(*explicit*)
};

} // namespace Cool