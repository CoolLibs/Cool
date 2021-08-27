#pragma once
#if defined(__COOL_APP_OPENGL)

#include "../RenderTargetInfo.h"
#include "TextureFB.h"

namespace Cool::OpenGL {

class RenderTarget {
public:
    void render(std::function<void()> render_fn);

    RenderTargetInfo info() const;
    uint32_t         width() const { return _texture.width(); }
    uint32_t         height() const { return _texture.height(); }
    // const Texture&   texture() const { return _texture.; }

    void imgui_window();

private:
    void resize(uint32_t width, uint32_t height);
    void resize_if_necessary();

private:
    TextureFB  _texture;
    glm::uvec2 _imgui_window_size{1, 1};
};

} // namespace Cool::OpenGL

#endif