#include "TextureView.h"
#include <WebGPU/webgpu.hpp>
#include "Cool/Gpu/WebGPUContext.h"

namespace Cool {

TextureView::TextureView(wgpu::TextureView handle)
    : WGPUUnique<wgpu::TextureView>{handle}
{}

} // namespace Cool