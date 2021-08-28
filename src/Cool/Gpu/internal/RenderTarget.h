#pragma once

#include "../RenderTargetInfo.h"

namespace Cool {

template<typename RenderTarget_Impl>
class RenderTarget_Base {
public:
    void render(RenderTarget_Impl::RenderFuncType render_fn)
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