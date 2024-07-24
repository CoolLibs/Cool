#include "TextureView.h"
#include <webgpu/webgpu.hpp>
#include "Cool/WebGPU/WebGPUContext.hpp"

namespace Cool {

TextureView::TextureView(wgpu::TextureView handle)
    : WGPUUnique<wgpu::TextureView>{handle}
{}

} // namespace Cool