#include "Cool/StrongTypes/Color.h"
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

img::Image RenderTarget_ImplOpenGL::download_pixels() const
{
    _texture.bind();
    std::unique_ptr<uint8_t[]> data{new uint8_t[4 * width() * height()]};
    glReadPixels(0, 0, static_cast<GLsizei>(width()), static_cast<GLsizei>(height()), GL_RGBA, GL_UNSIGNED_BYTE, data.get());
    _texture.unbind();
    return img::Image{
        img::Size{width(), height()},
        4,
        data.release()
    };
}

img::Image RenderTarget_ImplOpenGL::download_led_pixels() const
{
    _texture.bind();
    // std::unique_ptr<float[]> data_float{new float[3 * width()]};
    std::unique_ptr<uint8_t[]> data{new uint8_t[3 * width()]};
    glReadPixels(0, height() / 2, static_cast<GLsizei>(width()), 1, GL_RGB, GL_UNSIGNED_BYTE, data.get());
    // for (int i = 0; i < width(); i++)
    // {
    //     glm::vec3 linear = Cool::Color::from_srgb(glm::vec3{3 * i, 3 * i + 1, 3 * i + 2}).as_LinearRGB();
    //     data[3 * i + 0]  = static_cast<uint8_t>(linear.r * 255.f);
    //     data[3 * i + 1]  = static_cast<uint8_t>(linear.g * 255.f);
    //     data[3 * i + 2]  = static_cast<uint8_t>(linear.b * 255.f);
    // }
    _texture.unbind();
    return img::Image{
        img::Size{width(), 1},
        3,
        data.release()
    };
}

} // namespace Cool

#endif