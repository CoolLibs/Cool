#if defined(COOL_OPENGL)

#include "RenderTarget_ImplOpenGL.h"

namespace Cool {

RenderTarget_ImplOpenGL::RenderTarget_ImplOpenGL(img::Size size)
{
    resize(size);
}

void RenderTarget_ImplOpenGL::render(RenderFuncType render_fn)
{
    _texture.bind();
    render_fn();
    _texture.unbind();
}

RenderTargetInfo RenderTarget_ImplOpenGL::info() const
{
    return RenderTargetInfo{
        .viewport = {
            .size               = size(),
            .bottom_left_corner = {0, 0}
        }
    };
}

void RenderTarget_ImplOpenGL::resize(img::Size size)
{
    _texture.setSize(size);
}

} // namespace Cool

#endif