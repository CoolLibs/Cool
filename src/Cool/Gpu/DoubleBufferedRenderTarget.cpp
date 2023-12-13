#include "DoubleBufferedRenderTarget.h"

namespace Cool {

auto DoubleBufferedRenderTarget::read_target() const -> Cool::RenderTarget const&
{
    return _rt1_is_read
               ? _rt1
               : _rt2;
}
auto DoubleBufferedRenderTarget::read_target() -> Cool::RenderTarget&
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

void DoubleBufferedRenderTarget::clear_render_targets()
{
    _rt1.render([]() {
        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT);
    });
    _rt2.render([]() {
        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT);
    });
}

void DoubleBufferedRenderTarget::set_read_target_size_immediately(img::Size const& size)
{
    read_target().set_size_immediately(size);
}

void DoubleBufferedRenderTarget::swap_buffers()
{
    _rt1_is_read = !_rt1_is_read;
}

} // namespace Cool