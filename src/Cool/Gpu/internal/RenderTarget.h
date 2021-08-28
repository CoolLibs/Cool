#pragma once

#include "../OpenGL/RenderTarget.h"

namespace Cool {

using RenderTarget_Impl = Cool::OpenGL::RenderTarget;

class RenderTarget {
public:
    void render(std::function<void()> render_fn)
    {
        resize_if_necessary();
        _impl.render(render_fn);
    }

    RenderTargetInfo info() const { return _impl.info(); }
    uint32_t         width() const { return _impl.width(); }
    uint32_t         height() const { return _impl.height(); }

    void imgui_window() const;

private:
    void resize_if_necessary();

private:
    RenderTarget_Impl  _impl;
    mutable glm::uvec2 _imgui_window_size{1, 1};
};

} // namespace Cool