#if defined(__COOL_APP_OPENGL)

#include "RenderTarget_ImplOpenGL.h"

namespace Cool {

void RenderTarget_ImplOpenGL::render(RenderFuncType render_fn)
{
    _texture.bind();
    render_fn();
    _texture.unbind();
}

RenderTargetInfo RenderTarget_ImplOpenGL::info() const
{
    RenderTargetInfo info;
    info.viewport = RectSizePos{
        static_cast<int>(width()),
        static_cast<int>(height())};
    return info;
}

void RenderTarget_ImplOpenGL::resize(ImageSize size)
{
    _texture.setSize(size);
}

} // namespace Cool

#endif