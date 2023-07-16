#include "DoubleBufferedRenderTarget.h"

namespace Cool {

auto DoubleBufferedRenderTarget::read_target() -> Cool::RenderTarget const&
{
    return _rt1_is_read
               ? _rt1
               : _rt2;
}

auto DoubleBufferedRenderTarget::write_target() -> Cool::RenderTarget&
{
    return _rt1_is_read
               ? _rt2
               : _rt1;
}

void DoubleBufferedRenderTarget::swap_buffers()
{
    _rt1_is_read = !_rt1_is_read;
}

} // namespace Cool