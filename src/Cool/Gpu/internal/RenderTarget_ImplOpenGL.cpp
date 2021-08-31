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

ImageData RenderTarget_ImplOpenGL::download_pixels() const
{
    _texture.bind();
    std::unique_ptr<uint8_t[]> data{new uint8_t[4 * width() * height()]};
    glReadPixels(0, 0, width(), height(), GL_RGBA, GL_UNSIGNED_BYTE, data.get());
    _texture.unbind();
    return ImageData{
        ImageSize{width(), height()},
        4,
        std::move(data)};
}

} // namespace Cool

#endif