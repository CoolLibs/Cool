#include "TextureView.h"
#include <webgpu/webgpu.hpp>
#include "Cool/Gpu/WebGPUContext.h"

namespace Cool {

TextureView::TextureView(wgpu::TextureView handle)
    : WGPUUnique<wgpu::TextureView>{handle}
{}

} // namespace Cool