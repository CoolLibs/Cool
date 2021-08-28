#if defined(__COOL_APP_OPENGL)

#include "../RenderTarget_ImplOpenGL.h"

namespace Cool::OpenGL {

void RenderTarget::render(RenderFuncType render_fn)
{
    _texture.bind();
    render_fn();
    _texture.unbind();
}

RenderTargetInfo RenderTarget::info() const
{
    RenderTargetInfo info;
    info.viewport = RectSizePos{
        static_cast<int>(width()),
        static_cast<int>(height())};
    return info;
}

void RenderTarget::resize(uint32_t width, uint32_t height)
{
    _texture.setSize({width, height});
}

} // namespace Cool::OpenGL

#endif